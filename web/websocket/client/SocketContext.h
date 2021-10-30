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

#ifndef WEB_WS_CLIENT_SOCKETCONTEXT_H
#define WEB_WS_CLIENT_SOCKETCONTEXT_H

#include "web/websocket/SocketContext.h" // IWYU pragma: export

namespace net::socket::stream {
    class SocketConnection;
} // namespace net::socket::stream

namespace web::http::client {
    class Request;
    class Response;
} // namespace web::http::client

namespace web::websocket::client {
    class SubProtocol;
    class SocketContextUpgradeFactory;
} // namespace web::websocket::client

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <string> // for string

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::websocket::client {

    class SocketContext : public web::websocket::SocketContext<web::http::client::Request, web::http::client::Response, SubProtocol> {
    public:
        SocketContext(net::socket::stream::SocketConnection* socketConnection,
                      SocketContextUpgradeFactory* socketContextUpgradeFactory,
                      web::websocket::client::SubProtocol* subProtocol);

    protected:
        ~SocketContext() override;

    public:
        static SocketContext* create(SocketContextUpgradeFactory* socketContextUpgradeFactory,
                                     net::socket::stream::SocketConnection* socketConnection,
                                     const std::string& subProtocolName);

    protected:
        SocketContextUpgradeFactory* socketContextUpgradeFactory = nullptr;
    };

} // namespace web::websocket::client

#endif // WEB_WS_CLIENT_SOCKETCONTEXT_H
