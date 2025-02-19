/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021, 2022 Volker Christian <me@vchrist.at>
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

#ifndef NET_L2_STREAM_SERVERSOCKET_H
#define NET_L2_STREAM_SERVERSOCKET_H

#include "net/ServerSocket.h"     // IWYU pragma: export
#include "net/l2/stream/Socket.h" // IWYU pragma: export

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstdint> // IWYU pragma: export
#include <functional>
#include <string> // IWYU pragma: export

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::l2::stream {

    template <typename ConfigT>
    class ServerSocket : public net::ServerSocket<ConfigT, net::l2::stream::Socket> {
        using Super = net::ServerSocket<ConfigT, net::l2::stream::Socket>;

    protected:
        explicit ServerSocket(const std::string& name);

    public:
        using Super::listen;

        void listen(uint16_t psm, int backlog, const std::function<void(const SocketAddress&, int)>& onError);

        void listen(const std::string& address, int backlog, const std::function<void(const SocketAddress&, int)>& onError);

        void listen(const std::string& address,
                    uint16_t psm,
                    int backlog,
                    const std::function<void(const SocketAddress& SocketAddress, int)>& onError);
    };

} // namespace net::l2::stream

#endif // NET_L2_STREAM_SERVERSOCKET_H
