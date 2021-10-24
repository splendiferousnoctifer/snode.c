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

#include "web/websocket/client/SocketContextUpgradeFactory.h"

#include "utils/base64.h"
#include "web/http/client/Request.h"  // for Request
#include "web/http/client/Response.h" // for Response
#include "web/http/client/SocketContextUpgradeFactorySelector.h"
#include "web/websocket/client/SubProtocolFactory.h"
#include "web/websocket/client/SubProtocolFactorySelector.h" // for Sub...

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <unistd.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::websocket::client {

    void SocketContextUpgradeFactory::prepare(http::client::Request& request) {
        unsigned char ebytes[16];
        getentropy(ebytes, 16);

        request.set("Sec-WebSocket-Key", base64::base64_encode(ebytes, 16));
    }

    void SocketContextUpgradeFactory::deleteSubProtocol(SubProtocol* subProtocol) {
        SubProtocolFactory* subProtocolFactory = subProtocol->getSubProtocolFactory();

        if (subProtocolFactory->deleteSubProtocol(subProtocol) == 0) {
            SubProtocolFactorySelector::instance()->unload(subProtocolFactory);
        }

        --refCount;
        if (refCount == 0) {
            web::http::client::SocketContextUpgradeFactorySelector::instance()->unload(this);
        }
    }

    std::string SocketContextUpgradeFactory::name() {
        return "websocket";
    }

    SocketContext* SocketContextUpgradeFactory::create(net::socket::stream::SocketConnection* socketConnection) {
        std::string subProtocolName = response->header("sec-websocket-protocol");

        SocketContext* socketContext = nullptr;

        web::websocket::client::SubProtocolFactory* subProtocolFactory = SubProtocolFactorySelector::instance()->select(subProtocolName);

        if (subProtocolFactory != nullptr) {
            SubProtocol* subProtocol = subProtocolFactory->createSubProtocol();

            if (subProtocol != nullptr) {
                socketContext = new SocketContext(socketConnection, subProtocol);

                if (socketContext != nullptr) {
                    refCount++;

                    socketContext->setSocketContextUpgradeFactory(this);
                    subProtocol->setSocketContext(socketContext);
                    subProtocol->setSubProtocolFactory(subProtocolFactory);
                } else {
                    subProtocolFactory->deleteSubProtocol(subProtocol);
                }
            }
        }

        if (refCount == 0) {
            web::http::client::SocketContextUpgradeFactorySelector::instance()->unload(this);
        }

        return socketContext;
    }

    void SocketContextUpgradeFactory::link() {
        static bool linked = false;

        if (!linked) {
            web::http::client::SocketContextUpgradeFactorySelector::instance()->link("websocket", websocketClientContextUpgradeFactory);
            linked = true;
        }
    }

    extern "C" web::http::client::SocketContextUpgradeFactory* websocketClientContextUpgradeFactory() {
        return new SocketContextUpgradeFactory();
    }

} // namespace web::websocket::client
