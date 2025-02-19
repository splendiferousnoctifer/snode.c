/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "log/Logger.h"
#include "web/http/ConnectionState.h"
#include "web/http/http_utils.h"
#include "web/http/server/SocketContext.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::http::server {

    template <typename Request, typename Response>
    SocketContext<Request, Response>::SocketContext(core::socket::SocketConnection* socketConnection,
                                                    const std::function<void(Request&, Response&)>& onRequestReady)
        : Super(socketConnection)
        , onRequestReady(onRequestReady)
        , parser(
              this,
              [this](void) -> void {
                  VLOG(3) << "++ BEGIN:";

                  requestContexts.emplace_back(RequestContext(this));
              },
              [&requestContexts = this->requestContexts](const std::string& method,
                                                         const std::string& url,
                                                         const std::string& httpVersion,
                                                         int httpMajor,
                                                         int httpMinor,
                                                         const std::map<std::string, std::string>& queries) -> void {
                  VLOG(3) << "++ Request: " << method << " " << url << " " << httpVersion;

                  Request& request = requestContexts.back().request;

                  request.method = method;
                  request.url = url;
                  request.queries = &queries;
                  request.httpVersion = httpVersion;
                  request.httpMajor = httpMajor;
                  request.httpMinor = httpMinor;
              },
              [&requestContexts = this->requestContexts](const std::map<std::string, std::string>& header,
                                                         const std::map<std::string, std::string>& cookies) -> void {
                  Request& request = requestContexts.back().request;

                  VLOG(3) << "++ Header:";
                  request.headers = &header;

                  for (auto [field, value] : header) {
                      if (field == "connection" && httputils::ci_contains(value, "close")) {
                          request.connectionState = ConnectionState::Close;
                      } else if (field == "connection" && httputils::ci_contains(value, "keep-alive")) {
                          request.connectionState = ConnectionState::Keep;
                      }
                      VLOG(3) << "     " << field << ": " << value;
                  }

                  VLOG(3) << "++ Cookies";
                  request.cookies = &cookies;

                  for (auto [cookie, value] : cookies) {
                      VLOG(3) << "     " << cookie << ": " << value;
                  }
              },
              [&requestContexts = this->requestContexts](std::vector<uint8_t>& content) -> void {
                  VLOG(3) << "++ Content: ";

                  Request& request = requestContexts.back().request;

                  request.body = std::move(content);
              },
              [this]() -> void {
                  VLOG(3) << "++ Parsed ++";

                  RequestContext& requestContext = requestContexts.back();

                  requestContext.ready = true;

                  requestParsed();
              },
              [this](int status, const std::string& reason) -> void {
                  VLOG(3) << "++ Error: " << status << " : " << reason;

                  RequestContext& requestContext = requestContexts.back();

                  requestContext.status = status;
                  requestContext.reason = reason;
                  requestContext.ready = true;

                  close();
              }) {
    }

    template <typename Request, typename Response>
    void SocketContext<Request, Response>::onReceiveFromPeer() {
        parser.parse();
    }

    template <typename Request, typename Response>
    void SocketContext<Request, Response>::requestParsed() {
        if (!requestInProgress) {
            RequestContext& requestContext = requestContexts.front();

            requestInProgress = true;
            if (requestContext.status == 0) {
                if (((requestContext.request.connectionState == ConnectionState::Close) ||
                     (requestContext.request.httpMajor == 0 && requestContext.request.httpMinor == 9) ||
                     (requestContext.request.httpMajor == 1 && requestContext.request.httpMinor == 0 &&
                      requestContext.request.connectionState != ConnectionState::Keep) ||
                     (requestContext.request.httpMajor == 1 && requestContext.request.httpMinor == 1 &&
                      requestContext.request.connectionState == ConnectionState::Close))) {
                    requestContext.response.set("Connection", "close");
                } else {
                    requestContext.response.set("Connection", "keep-alive");
                }

                onRequestReady(requestContext.request, requestContext.response);
            } else {
                requestContext.response.status(requestContext.status).send(requestContext.reason);
                shutdownWrite();
            }
        }
    }

    template <typename Request, typename Response>
    void SocketContext<Request, Response>::sendToPeerCompleted() {
        RequestContext& requestContext = requestContexts.front();

        // if 0.9 => terminate
        // if 1.0 && (request != Keep || contentLength = -1) => terminate
        // if 1.1 && (request == Close || contentLength = -1) => terminate
        // if (request == Close) => terminate

        if ((requestContext.request.httpMajor == 0 && requestContext.request.httpMinor == 9) ||
            (requestContext.request.httpMajor == 1 && requestContext.request.httpMinor == 0 &&
             requestContext.request.connectionState != ConnectionState::Keep) ||
            (requestContext.request.httpMajor == 1 && requestContext.request.httpMinor == 1 &&
             requestContext.request.connectionState == ConnectionState::Close) ||
            (requestContext.response.connectionState == ConnectionState::Close)) {
            shutdownWrite();
        } else {
            reset();

            requestContexts.pop_front();

            if (!requestContexts.empty() && requestContexts.front().ready) {
                RequestContext& requestContext = requestContexts.front();

                requestInProgress = true;
                if (requestContext.status == 0) {
                    onRequestReady(requestContext.request, requestContext.response);
                } else {
                    requestContext.response.status(requestContext.status).send(requestContext.reason);
                    shutdownWrite();
                }
            }
        }
    }

    template <typename Request, typename Response>
    void SocketContext<Request, Response>::reset() {
        if (!requestContexts.empty()) {
            RequestContext& requestContext = requestContexts.front();
            requestContext.request.reset();
            requestContext.response.reset();
        }

        contentSent = 0;
        contentLength = 0;

        requestInProgress = false;
    }

    template <typename Request, typename Response>
    void SocketContext<Request, Response>::SocketContext::onConnected() {
        VLOG(0) << "HTTP connected";
    }

    template <typename Request, typename Response>
    void SocketContext<Request, Response>::onDisconnected() {
        VLOG(0) << "HTTP disconnected";
    }

} // namespace web::http::server
