/**
 * \file socket-client.hpp
 * Client Class Structure
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

#include "socket.hpp"
#pragma once

namespace ipc
{
namespace net
{
/**
 * @class Client.
 * @brief This class is wrapper around Client-sided socket.
 * @details In all used syscalls in this class; as error occurrence ,logs will be printed in debug
 * or warning mode includes errno message; unless an exception is thrown.
 *
 * @warning after creating a socket, you MUST close it explicity.
 *
 * @note errno will be set after failure of any method who uses syscalls.
 */
class Client : public Socket
{
public:
    /**
     * @brief Default Constructor, creates a client socket.
     *
     * @param pname for XmixParam class
     */
    Client(const string &pname = "socket");
    /**
     * @brief Construct a new Client object.
     *
     * @param domain Socket's Domain
     * @param type Socket's Type
     * @param pname for XmixParam class
     */
    Client(const SockDom::domain domain, const SockType::type type, const string &pname = "socket");
    /**
     * @brief Copy constructor.
     *
     * @param clnt reference to an instance of Client
     * @warning This constructor copies FD as well as other attributes.
     */
    Client(const Client &clnt);
    /**
     * @brief connects to address field.
     *
     * @param timeout optional parameter for setting timeout (in milisecond) for checkTimeout()
     * method.
     *
     * @details this method causes the process to wait until a successful connection or timeout
     * expires (clearly when timeout already set).
     *
     * @warning this method throws an exception on error
     */
    virtual void connect(const int timeout = 0);

private:
    /**
     * @brief connecting to unix socket domain.
     *
     * @warning on error occurrance, errno will be set accordingly.
     */
    void connectToUNIX();
    /**
     * @brief connecting to ipv4 socket domain.
     *
     * @warning on error occurrance, errno will be set accordingly.
     */
    void connectToIN4();
    /**
     * @brief connecting to ipv6 socket Domain.
     *
     * @warning on error occurrance, errno will be set accordingly.
     */
    void connectToIN6();
};
} // namespace net
} // namespace ipc
