/**
 * \file socket-types.hpp
 * Socket types Structures.
 *
 * \author Nasrolahi (nasrollahi@acloudavid.com)
 *
 * ipc is free software: you can redistribute it and/or modify it
 * under the terms of GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the license, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with foobar. If not, see <http://www.gnu.org/licenses/>
 */

#pragma once
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <pparam/sparam.hpp>
#include <pparam/xparam.hpp>
#include <string.h>
#include <string>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>
#include <utility> // pair definition
#include <vector>

namespace ipc
{
namespace net
{
typedef std::pair<std::string, int32_t> Address;
class SockDom
{
public:
    enum domain
    {
        UNIX,
        IPV4,
        IPV6,
        MAX
    };
    static const std::string typeString[MAX];
};
class SockType
{
public:
    enum type
    {
        TCP,
        UDP,
        MAX
    };
    static const std::string typeString[MAX];
};
} // namespace net
} // namespace ipc
