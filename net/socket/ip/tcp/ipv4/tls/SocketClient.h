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

#ifndef NET_SOCKET_IP_TCP_IPV4_TLS_SOCKETCLIENT_H
#define NET_SOCKET_IP_TCP_IPV4_TLS_SOCKETCLIENT_H

#include "net/socket/ip/tcp/ipv4/Socket.h"
#include "net/socket/stream/tls/SocketClient.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif // DOXYGEN_SHOULD_SKIP_THIS

namespace net::socket::ip::tcp::ipv4::tls {

    template <typename SocketContextFactoryT>
    class SocketClient : public net::socket::stream::tls::SocketClient<ipv4::Socket, SocketContextFactoryT> {
        using net::socket::stream::tls::SocketClient<ipv4::Socket, SocketContextFactoryT>::SocketClient;

    public:
        using SocketAddress = typename net::socket::stream::tls::SocketClient<ipv4::Socket, SocketContextFactoryT>::SocketAddress;

        using net::socket::stream::tls::SocketClient<ipv4::Socket, SocketContextFactoryT>::connect;

        void connect(const std::string& ipOrHostname, uint16_t port, const std::function<void(int)>& onError) {
            connect(SocketAddress(ipOrHostname, port), onError);
        }

        void connect(const std::string& ipOrHostname,
                     uint16_t port,
                     const std::string& bindIpOrHostname,
                     const std::function<void(int)>& onError) {
            connect(SocketAddress(ipOrHostname, port), bindIpOrHostname, onError);
        }

        void connect(const std::string& ipOrHostname,
                     uint16_t port,
                     const std::string& bindIpOrHostname,
                     uint16_t bindPort,
                     const std::function<void(int)>& onError) {
            connect(SocketAddress(ipOrHostname, port), SocketAddress(bindIpOrHostname, bindPort), onError);
        }
    };

} // namespace net::socket::ip::tcp::ipv4::tls

#endif // NET_SOCKET_IP_TCP_IPV4_TLS_SOCKETCLIENT_H
