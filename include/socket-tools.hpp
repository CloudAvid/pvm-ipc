/**
 * \file socket-tools.hpp
 * Utilities related to Sockets.
 *
 * \author Ali Nasrolahi (nasrollahi@acloudavid.com)
 *
 * ipc is free software: you can redistribute it and/or modify it
 * under the terms of GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the license, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with pvm-ipc. If not, see <http://www.gnu.org/licenses/>
 */
#include "socket-struct.hpp"

namespace ipc::net
{
/**
 * @class SockTools.
 * @brief Need set of tools to work with sockets.
 */
class SockTools
{
public:
    /**
     * @brief Converts path to unix socket address structure.
     * @param path Path of socket.
     * @return native structure.
     */
    static sockaddr_un convertAddrToNativeUNIX(const char *path);

    /**
     * @brief Converts IP,Port combination to IPv4 address structure.
     * @param addr Address.
     * @return native structure.
     */
    static sockaddr_in convertAddrToNativeIP4(Address addr);

    /**
     * @brief Converts IP,Port combination to IPv6 address structure.
     * @param addr Address.
     * @return native structure.
     */
    static sockaddr_in6 convertAddrToNativeIP6(Address addr);

    /**
     * @brief Converts native unix address structre to string.
     * @param addr native structure.
     * @return address in string format.
     */
    static std::string convertNativeToAddr(sockaddr_un addr);

    /**
     * @brief Converts native IPv4 address structre to IP,port combo.
     * @param addr native structure.
     * @return IP,Port pair.
     */
    static Address convertNativeToAddr(sockaddr_in addr);

    /**
     * @brief Converts native IPv6 address structre to IP,port combo.
     * @param addr native structure.
     * @return IP,Port pair.
     */
    static Address convertNativeToAddr(sockaddr_in6 addr);
};

} // namespace ipc::net
