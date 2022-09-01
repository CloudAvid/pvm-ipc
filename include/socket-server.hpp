/**
 * \file socket-server.hpp
 * Server Class Structure
 *
 * \author Ali Nasrolahi (nasrollahi@acloudavid.com)
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
#include "socket.hpp"

namespace ipc
{
namespace net
{

/**
 * @class Server
 *
 * @note used to create server-sided sockets.
 *
 * @details In all used syscalls in this class; as error occurrence, logs will be printed in debug
 * mode includes errno message; Unless an exception is thrown.
 *
 * @note errno will be set after failure of each method who use syscalls.
 */
class Server : public Socket
{
public:
    /**
     * @brief default constructor.
     *
     * @param pname tag for XMixParam class.
     */

    Server(const string &pname = "socket");
    /**
     * @brief Construct a new Server object.
     *
     * @param domain Socket's Domain
     * @param type Socket's Type
     */
    Server(const SockDom::domain domain, const SockType::type type, const string &pname = "socket");
    /**
     * @brief Copy Constructor.
     * @param srvr is a reference to Server obj.
     * @warning This constructor copies FD as well as other attributes.
     */
    Server(const Server &srvr);

    /**
     * @brief bind socket to address.
     *
     * @warning this method throws an exception on error.
     */
    void bind();
    /**
     * @brief listening on address.
     *
     * @warning this method throws an exception on error.
     */
    void listen();
    /**
     * @brief accepts new connections.
     *
     * @param acceptedSocket is value-result argument.
     *
     * @details acceptedSocket's FD will be set after successfully
     * a new connection get accepted.
     *
     * @note By calling this method, your proccess
     * would wait until a new connection accepted
     * or recieves an interuption.
     *
     * @warning this method throws an exception on error.
     *
     * @warning SOCK_CLOEXEC flag is set by defualt
     * for accepted socket (smiliar to Socket Class).
     * @see  "Socket class's description for how to unset this flag"
     */
    void accept(Server &acceptedSocket);

    void set_backlog(const int backlog);

    int get_backlog() const;

private:
    /**
     * @brief accepts new connections.
     *
     * @return accepted fd or throws an exception on error.
     *
     * @details By calling this method, your proccess
     * would wait until a new connection accepted
     * or recieves an interuption.
     *
     * @warning this method throws an exception on error.
     *
     * @warning SOCK_CLOEXEC flag is set by defualt
     * for accepted socket (smiliar to Socket Class).
     * @see  "Socket class's description for how to unset this flag"
     */
    int accept();

    /**
     * @brief bind to unix path.
     *
     * @details calling bind syscall with appropriate settings and configs for unix path.
     * @warning this method throws an exception on error.
     *
     * @note Address which provided for this method better be an absolute path, even though
     * relative path wouldn't fail.
     */
    void bindToUNIX();
    /**
     * @brief bind to IPv4.
     *
     * @details calling bind syscall with appropriate settings and configs for IPv4.
     * @warning this method throws an exception on error.
     */
    void bindToIN4();
    /**
     * @brief bind to IPv6.
     *
     * @details calling bind syscall with appropriate settings and configs for IPv6.
     * @warning this method throws an exception on error.
     */
    void bindToIN6();

private:
    /**
     * @brief This field is backlog param for listen syscall.
     */
    XIntParam<XUInt> backlog;
};
} // namespace net
} // namespace ipc
