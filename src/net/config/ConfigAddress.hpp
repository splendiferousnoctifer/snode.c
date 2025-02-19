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

#ifndef NET_CONFIG_CONFIGADDRESS_HPP
#define NET_CONFIG_CONFIGADDRESS_HPP

#include "ConfigAddress.h" // IWYU pragma: export

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "utils/CLI11.hpp"

#endif // DOXYGEN_SHOULD_SKIP_THIS

namespace net::config {

    template <typename SocketAddress>
    ConfigAddress<SocketAddress>::ConfigAddress(const std::string& addressOptionName, const std::string& addressOptionDescription) {
        if (!getName().empty()) {
            addressSc = add_subcommand(addressOptionName, addressOptionDescription);
            addressSc->group("Option groups");
            needsRequired = true;
            initialized = true;
        }
    }

    template <typename SocketAddress>
    const SocketAddress& ConfigAddress<SocketAddress>::getAddress() {
        if (addressSc != nullptr) {
            if (needsRequired) {
                parse(true); // Try command line parsing honoring required options
                updateFromCommandLine();
                needsRequired = false;
                updated = true;
            } else if (!updated) {
                updateFromCommandLine();
                updated = true;
            }
        }

        return address;
    }

    template <typename SocketAddress>
    void ConfigAddress<SocketAddress>::setAddress(const SocketAddress& localAddress) {
        address = localAddress;
        initialized = true;
        needsRequired = false;
    }

    template <typename SocketAddressT>
    bool ConfigAddress<SocketAddressT>::isInitialized() const {
        return initialized;
    }

    template <typename SocketAddress>
    void ConfigAddress<SocketAddress>::require(CLI::Option* opt) {
        addressSc->required();
        opt->required();
    }

    template <typename SocketAddress>
    void ConfigAddress<SocketAddress>::require(CLI::Option* opt1, CLI::Option* opt2) {
        addressSc->required();
        opt1->required();
        opt2->required();
    }

} // namespace net::config

#endif // NET_CONFIG_CONFIGADDRESS_HPP
