/**
 * \file socket.hpp
 * Socket Class Structure
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

#pragma once
#include "socket-struct.hpp"
#include "socket-tools.hpp"

namespace ipc::net
{
/**
 * @class Socket
 * @brief Responsible for creating and handling sockets.
 * @details In all used syscalls in this class; as error occurrence, logs will be printed in debug
 * or warning mode includes errno message; unless an exception is thrown.
 *
 * @note errno will be set for all methods wchich using syscalls.
 *
 * @note CLOEXEC flag is set for fd by defualt, in rare cases which is
 * needed to be removed use and unsetFDFlags(); as shown below:
 * @code
 *      // after socket creation.
 *      unsetFDFlags(FD_CLOEXEC);
 * @endcode
 *
 * @warning unseting CLOEXEC is not recommanded.
 *
 * @note by default we assume socket is block. However methods ,who
 * use timeout, set nonblock flag for fd explicity then unset it as return.
 *
 * @warning after creating a socket (successful return from bind() or connect()),
 * you MUST close it explicity.
 *
 * @note Datagram transmissions (SOCK_DGRAM) in unix domain (AF_UNIX) is reliable,
 * in spite of IPv4 (AF_INET) and IPv6 (AF_INET6) domains.
 *
 */
class Socket : public XMixParam
{
public:
    /**
     * @brief Construct a new Socket object.
     * @param pname indicates xml tag for XMixParam class
     * @details IPV4 Domain and TCP type are defualt value.
     */
    Socket(const string &pname = "socket");

    /**
     * @brief Construct a new Socket object.
     *
     * @param domain Specify Domain UNIX, IPV4, IPV6.
     * @param type Specify Type TCP, UDP.
     * @param pname indicates xml tag for XMixParam class
     */
    Socket(const SockDom::domain domain, const SockType::type type, const string &pname = "socket");

    /**
     * @brief Copy constructor.
     *
     * @param Socket reference to Socket object.
     * @warning This constructor copies FD as well as other attributes.
     */
    Socket(const Socket &Socket);

    /**
     * @brief This method set socket options.
     *
     * @param optionLevel will set option level param in setsockopt.
     * @param optionName will set option name param in setsockopt.
     * @param optionVal will set option val param in setsockopt.
     * @param len is optionVal length
     * @warning this method throws an exception on error.
     */
    void setSockopt(const int optionLevel, const int optionName, const void *optionVal,
                    socklen_t len);

    /**
     * @brief This method get socket options.
     *
     * @param optionLevel will set option level param in getsockopt
     * @param optionName will set option name param in getsockopt
     * @param optionVal is the buffer which will be filled with result of getsockopt.
     * @param len is the size of optionVal parameter.
     *
     * @warning this method throws an exception on error.
     */
    void getSockopt(const int optionLevel, const int optionName, void *optionVal, socklen_t *len);

    /**
     * @brief Get name of connected peer socket.
     *
     * @return address of peer socket or.
     * @warning throws an exception on error.
     */
    Address getPeerName();

    /**
     * @brief writes buffer to socket.
     *
     * @param buffer need to be written to socket.
     * @param size size of buffer.
     * @return written bytes or -1 on Error.
     *
     * @note on error check errno for full description.
     */
    virtual const ssize_t write(const char *buffer, const size_t size);

    /**
     * @brief writes buffer to socket.
     *
     * @param buffer need to be written to socket.
     * @param size size of buffer.
     * @param timeout will takes timeout in milisecond
     * @return written bytes.
     *
     * @warning this method throws exception on error.
     *
     * @warning this method non-block socket and block it by each call.
     */
    virtual const ssize_t write(const char *buffer, const size_t size, const int timeout);

    /**
     * @brief writes buffer to socket.
     *
     * @param buffer need to be written to socket.
     * @return written bytes or -1 on Error.
     *
     * @note on error check errno for full description.
     */
    virtual const ssize_t write(const std::string &buffer);

    /**
     * @brief writes buffer to socket.
     *
     * @param buffer need to be written to socket.
     * @param size size of buffer.
     * @param flags flags for send operation.
     * @return written bytes or -1 on Error.
     *
     * @note on error, check errno for full description.
     */
    virtual const ssize_t send(const char *buffer, const size_t size, const int flags);

    /**
     * @brief writes buffer to socket.
     *
     * @param buffer need to be written to socket.
     * @param size size of buffer.
     * @param flags flags for send operation.
     * @param addr address of peer.
     * @param len length of peer address.
     * @return written bytes or -1 on Error.
     *
     * @note on error, check errno for full description.
     *
     * @warning this method creates FD for transmission explicity,
     */
    virtual const ssize_t sendto(const char *buffer, const size_t size, const int flags);

    /**
     * @brief Writes buffer to socket "fully".
     *
     * @param buffer message which should be sent.
     *
     * @note this method is smiliar to reguler write except, when
     * write fails to send buffer completely, it'll retries repeatedly untill successful transfer.
     *
     * @warning this method throws an exception on error.
     * @warning this method potentially could be block "infinitely".
     */
    virtual void fullWrite(const std::string &buffer);

    /**
     * @brief Writes buffer to socket "fully".
     *
     * @param buffer message which should be sent.
     * @param size is buffer size.
     *
     * @note this method is smiliar to regular  write except, when
     * write fails to send buffer completely, it'll retries repeatedly untill successful transfer.
     *
     * @warning this method throws an exception on error.
     * @warning this method potentially could be block "infinitely".
     */
    virtual void fullWrite(const char *buffer, const size_t size);

    /**
     * @brief read from socket by size of Buffsize.
     *
     * @param buffer for read syscall.
     * @param buffSize size of buffer.
     * @param timeout takes timeout in milisecond
     * @return read bytes.
     *
     * @warning this method throws exception on error.
     *
     * @warning this method non-block socket and block it by each call.
     */
    virtual const ssize_t read(char *buffer, const size_t buffSize, const int timeout);

    /**
     * @brief read from socket by size of Buffsize.
     *
     * @param buffer for read syscall.
     * @param buffSize size of buffer.
     * @return read bytes or -1 on Error.
     *
     * @note on error, check errno for full description.
     */
    virtual const ssize_t read(char *buffer, const size_t buffSize);

    /**
     * @brief read from socket by size of Buffsize.
     *
     * @param buffer for read syscall.
     * @param buffSize size of buffer.
     * @param flags flags for recv operation.
     * @return read bytes or -1 on Error.
     *
     * @note on error, check errno for full description.
     */
    virtual const ssize_t recv(char *buffer, const size_t size, const int flags);

    /**
     * @brief read from socket by size of Buffsize.
     *
     * @param buffer for read syscall.
     * @param buffSize size of buffer.
     * @param flags flags for recv operation.
     * @param addr address of peer.
     * @param len length of peer address.
     * @return read bytes or -1 on Error.
     *
     * @note on error, check errno for full description.
     *
     * @warning this method creates FD for transmission explicity,
     */
    virtual const ssize_t recvfrom(char *buffer, const size_t size, const int flags);

    /**
     * @brief reads until EOF or intteruptions.
     *
     * @param buffer result-value of read data.
     * @warning throws an exception on error.
     */
    virtual void fullRead(string &buffer);

    /**
     * @brief close socket.
     *
     * @details this method set fd field to zero as well as closing socket.
     */
    virtual void close();

    /**
     * @brief checking if socket is still open.
     *
     * @return true if socket is still open and false otherwise.
     * @details isOpen method determines fd status according to fd value ,zero indicates closed fd.
     * */
    bool isOpen() const;

    /**
     * @brief Set file status flags.
     *
     * @param bitmask which flags to set.
     * @warning this method throws an exception on error.
     * @see fcntl(2) and difference between SETFD and SETFL.
     */
    void setFSFlags(const int bitmask);

    /**
     * @brief Unset file status flags.
     *
     * @param bitmask which flags to unset.
     *
     * @warning this method throws an exception on error.
     * @see fcntl(2) and difference between SETFD and SETFL.
     */
    void unsetFSFlags(const int bitmask);

    /**
     * @brief Set file descriptor flags.
     *
     * @param bitmask which flags to set.
     * @warning this method throws an exception on error.
     * @see fcntl(2) and difference between SETFD and SETFL.
     */
    void setFDFlags(const int bitmask);

    /**
     * @brief Unset file descriptor flags.
     *
     * @param bitmask which flags to unset.
     *
     * @warning this method throws an exception on error.
     * @see fcntl(2) and difference between SETFD and SETFL.
     */
    void unsetFDFlags(const int bitmask);

    /**
     * @brief Shutdowns socket.
     *
     * @param how specefies what type of communication
     * should be shutdown.
     * @note please study shutdown(2) for valid values and
     * meanings of each flag.
     * @warning this method throws an exception on error.
     */
    void shutdown(const int how);

    /**
     * @warning none of socket's parameters cannot be
     * changed unless socket is closed.
     */

    void set_protocol(const int protocol);
    void set_domain(const int domain);
    void set_type(const int type);
    void set_unixAddr(const std::string &unixAddr);
    void set_ip(const std::string &ip);
    void set_port(const unsigned int port);

    /**
     * @brief Set address.
     *
     * @param addr indicates the pair of ip and port
     * @details this method will be vaild only in IPV4 or IPV6 domain
     */
    void setAddr(const Address addr);

    /**
     * @brief Set address.
     *
     * @param addr indicates unix path.
     * @details this method will be vaild only in unix path.
     */
    void setAddr(const std::string &addr);

    /**
     * @brief get already set status flags.
     *
     * @return status flags which are set or
     *
     * @warning throws an exception on error.
     * @see fcntl(2) and difference between GETFD and GETFL.
     */
    const int getFSFlags();

    /**
     * @brief get already set file descriptor flags.
     *
     * @return FD flags which are set or
     *
     * @warning throws an exception on error.
     * @see fcntl(2) and difference between GETFD and GETFL.
     */
    const int getFDFlags();

    int get_fd() const;
    int get_protocol() const;
    int get_domain() const;
    int get_type() const;
    std::string get_ip() const;
    unsigned int get_port() const;
    std::string get_unixAddr() const;

    /**
     * @brief returns Address.
     *
     * @return an address. This address pair which is set to ip and
     * port when IPvx domain is defined or unix path while unix domain
     * selected.
     * @warning If unix domain is select second param (port) is set to -1.
     *
     */
    Address getAddr() const;

    sockaddr getNativeAddr();

protected:
    void set_fd(const int fd);

    /**
     * @brief Create socket.
     *
     * @warning this method throws exceptions if fd creation fails due
     * to socket() syscall failure or fd field has non-zero value.
     */
    void create_fd();

    /**
     * @brief checks timeout.
     *
     * @details This method using polling to check for ready sockets.
     *
     * @warning this method throws exceptions on error
     */
    virtual void checkTimeout(const int timeout, const uint32_t flags);
    /**
     * @brief unset non-block flag for fd.
     *
     * @return 0 on success or -1 on error.
     * @note this method implemented as a
     * no-exception equivalant for seFDFlags.
     */
    int _blockFD();
    /**
     * @brief set non-block flag for fd.
     *
     * @return 0 on success or -1 on error.
     * @note this method implemented as a
     * no-exception equivalant for seFDFlags.
     */
    int _nonblockFD();

    /**
     * @brief sets specific type, flags.
     *
     * @param type is which type of flags to set.
     * @param bitmask indicates bitmask of flags.
     *
     * @warning This method throws an exception on error.
     */
    void setFlags(const int type, const int bitmask);

    /**
     * @brief gets specific type, flags.
     *
     * @param type is which type of flags to get.
     *
     * @warning This method throws an exception on error.
     */
    int getFlags(const int type);

    /**
     * @brief get peer name in UNIX domain.
     *
     * @return peer address or
     * @warning throws an exception on error.
     */
    Address getUNIXpeer();

    /**
     * @brief get peer name in IPv4 domain.
     *
     * @return peer address or
     * @warning throws an exception on error.
     */
    Address getIN4peer();

    /**
     * @brief get peer name in IPv6 domain.
     *
     * @return peer address or
     * @warning throws an exception on error.
     */
    Address getIN6peer();

    /**
     * @brief fills up nativeUNIXaddr field.
     *
     */
    void create_nativeUNIXaddr();

    /**
     * @brief fills up nativeIP4addr field.
     *
     */
    void create_nativeIP4addr();

    /**
     * @brief fills up nativeIP6addr field.
     *
     */
    void create_nativeIP6addr();

protected:
    /**
     * @brief Socket's FD.
     */
    XIntParam<XInt> fd;
    /**
     * @brief Domain of socket
     */
    XEnumParam<SockDom> domain;
    /**
     * @brief Type of socket
     */
    XEnumParam<SockType> type;
    /**
     * @brief Protocol of socket
     */
    XIntParam<XInt> protocol;

    /**
     * @brief ip of socket.
     *
     * @note this ip will be used for "listening on" for Server Sockets
     * or "Connecting to" for Client Sockets.
     *
     * @details ip is implemented only for Ipv4 or Ipv6 domain.
     */
    IPxParam ip;
    /**
     * @brief port of socket.
     *
     * @note this port will be used for "listening on" for Server Sockets
     * or "Connecting to" for Client Sockets.
     * @details port is implemented only for Ipv4 or Ipv6 domain.
     */
    XIntParam<XUInt> port;
    /**
     * @brief unix address of socket.
     *
     * @note this path will be used for "listening on" for Server Sockets
     * or "Connecting to" for Client Sockets.
     * @details unixAddr is implemented only for unix domain.
     */
    XTextParam unixAddr;

    /**
     * @brief native address of socket.
     *
     */
    sockaddr_un nativeUNIXaddr;

    /**
     * @brief native address of socket.
     *
     */
    sockaddr_in nativeIP4addr;

    /**
     * @brief native address of socket.
     *
     */
    sockaddr_in6 nativeIP6addr;
};
} // namespace ipc::net
