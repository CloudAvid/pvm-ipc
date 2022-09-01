#include "socket.hpp"

#include "plogger.hpp"

using namespace plogger;
namespace ipc
{
namespace net
{
Socket::Socket(const string &pname) :
    XMixParam(pname),
    fd("fd", 0, -1),
    domain("domain", 0),
    type("type", 0),
    protocol("protocol", 0, -1),
    ip("ip"),
    port("port", 0, -1),
    unixAddr("unixAddr")
{
    addParam(&fd);
    addParam(&protocol);
    addParam(&type);
    addParam(&domain);
    addParam(&ip);
    addParam(&port);
    addParam(&unixAddr);

    /* Defualt values */
    set_domain(SockDom::domain::IPV4);
    set_type(SockType::type::TCP);
}

Socket::Socket(const Socket &socket) : Socket(socket.get_pname())
{
    set_protocol(socket.get_protocol());
    set_domain(socket.get_domain());
    set_type(socket.get_type());
    set_ip(socket.get_ip());
    set_unixAddr(socket.get_unixAddr());
    set_port(socket.get_port());
    set_fd(socket.get_fd());
}

Socket::Socket(const SockDom::domain domain, const SockType::type type, const string &pname) :
    Socket(pname)
{
    CALL_FUNCTION;

    set_domain(domain);
    set_type(type);

    EXIT_FUNCTION;
}

void Socket::create_fd()
{
    CALL_FUNCTION;

    if (isOpen()) {
        EXIT_FUNCTION_THROW(SOCKET_CREATION_FAILED_IS_OPEN, fd.get_value());
    }
    int _type, _domain;

    switch (get_type()) {
    case SockType::type::TCP:
        _type = SOCK_STREAM;
        break;
    case SockType::type::UDP:
        _type = SOCK_DGRAM;
        break;
    default:
        EXIT_FUNCTION_THROW(SOCKET_UNDEFINED_TYPE);
    }

    switch (get_domain()) {
    case SockDom::domain::UNIX:
        _domain = AF_UNIX;
        create_nativeUNIXaddr();
        break;
    case SockDom::domain::IPV4:
        _domain = AF_INET;
        create_nativeIP4addr();
        break;
    case SockDom::domain::IPV6:
        _domain = AF_INET6;
        create_nativeIP6addr();
        break;
    default:
        EXIT_FUNCTION_THROW(SOCKET_UNDEFINED_DOMAIN);
    }

    fd = ::socket(_domain, _type | SOCK_CLOEXEC, protocol.get_value());

    if (fd.get_value() < 0) {
        EXIT_FUNCTION_THROW(SOCKET_CREATION_FAILED, strerror(errno));
    }

    PLOG(Severity::DEBUG, SOCKET_CREATION_SUCCESS, fd.get_value());
    EXIT_FUNCTION;
}

void Socket::create_nativeUNIXaddr()
{
    nativeUNIXaddr = SockTools::convertAddrToNativeUNIX(get_unixAddr().c_str());
}

void Socket::create_nativeIP4addr()
{
    nativeIP4addr = SockTools::convertAddrToNativeIP4(getAddr());
}

void Socket::create_nativeIP6addr()
{
    nativeIP6addr = SockTools::convertAddrToNativeIP6(getAddr());
}

const ssize_t Socket::write(const char *buffer, const ssize_t size)
{
    ssize_t res = ::write(get_fd(), buffer, size);
    if (res < 0) {
        PLOG(Severity::DEBUG, SOCKET_WRITE_FAILED, fd.get_value(), strerror(errno));
    }
    return res;
}

const ssize_t Socket::write(const std::string &buffer)
{
    return write(buffer.c_str(), buffer.size());
}

const ssize_t Socket::write(const char *buffer, const ssize_t size, const int timeout)
{
    CALL_FUNCTION;
    int status;
    try {
        setFSFlags(O_NONBLOCK);
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }

    try {
        status = write(buffer, size);
        if (status < -1 && (errno == EWOULDBLOCK || errno == EAGAIN)) {
            checkTimeout(timeout, EPOLLOUT);
        }
        unsetFSFlags(O_NONBLOCK);
    } catch (Exception &e) {
        if (_blockFD() < 0) {
            PLOG(Severity::DEBUG, SOCKET_FCNTL_NONBLOCK_FAILED, fd.get_value(), strerror(errno));
        }
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }
    EXIT_FUNCTION_RETURN(status);
}

const ssize_t Socket::send(const char *buffer, const size_t size, const int flags)
{
    ssize_t res = ::send(get_fd(), buffer, size, flags);
    if (res < 0) {
        PLOG(Severity::DEBUG, SOCKET_WRITE_FAILED, fd.get_value(), strerror(errno));
    }
    return res;
}

const ssize_t Socket::sendto(const char *buffer, const size_t size, const int flags)
{
    if (! isOpen()) {
        create_fd();
    }

    ssize_t res = 0;
    sockaddr *addr;
    switch (get_domain()) {
    case SockDom::domain::UNIX:
        addr = (sockaddr *) &nativeUNIXaddr;
        break;
    case SockDom::domain::IPV4:
        addr = (sockaddr *) &nativeUNIXaddr;
        break;
    case SockDom::domain::IPV6:
        addr = (sockaddr *) &nativeUNIXaddr;
        break;
    default:
        EXIT_FUNCTION_THROW(SOCKET_UNDEFINED_DOMAIN);
    }

    if ((res = ::sendto(get_fd(), buffer, size, flags, addr, sizeof addr)) < 0) {
        PLOG(Severity::DEBUG, SOCKET_WRITE_FAILED, fd.get_value(), strerror(errno));
    }
    return res;
}

const ssize_t Socket::read(char *buffer, const int buffSize)
{
    memset(buffer, 0, buffSize);
    ssize_t res = ::read(get_fd(), buffer, buffSize - 1);
    if (res < 0) {
        PLOG(Severity::DEBUG, SOCKET_READ_FAILED, fd.get_value(), strerror(errno));
    }
    return res;
}

const ssize_t Socket::read(char *buffer, const int buffSize, const int timeout)
{
    CALL_FUNCTION;
    int status;
    try {
        setFSFlags(O_NONBLOCK);
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }
    try {
        status = read(buffer, buffSize);
        if (status < -1 && (errno == EWOULDBLOCK || errno == EAGAIN)) {
            checkTimeout(timeout, EPOLLIN);
        }
        unsetFSFlags(O_NONBLOCK);
    } catch (Exception &e) {
        if (_blockFD() < 0) {
            PLOG(Severity::DEBUG, SOCKET_FCNTL_NONBLOCK_FAILED, fd.get_value(), strerror(errno));
        }
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }
    EXIT_FUNCTION_RETURN(status);
}

const ssize_t Socket::recv(char *buffer, const size_t size, const int flags)
{
    memset(buffer, 0, size);
    ssize_t res = ::recv(get_fd(), buffer, size, flags);
    if (res < 0) {
        PLOG(Severity::DEBUG, SOCKET_READ_FAILED, fd.get_value(), strerror(errno));
    }
    return res;
}

const ssize_t Socket::recvfrom(char *buffer, const size_t size, const int flags)
{
    if (! isOpen()) {
        create_fd();
    }

    ssize_t res = 0;
    sockaddr *addr;
    switch (get_domain()) {
    case SockDom::domain::UNIX:
        addr = (sockaddr *) &nativeUNIXaddr;
        break;
    case SockDom::domain::IPV4:
        addr = (sockaddr *) &nativeUNIXaddr;
        break;
    case SockDom::domain::IPV6:
        addr = (sockaddr *) &nativeUNIXaddr;
        break;
    default:
        EXIT_FUNCTION_THROW(SOCKET_UNDEFINED_DOMAIN);
    }

    auto len = (socklen_t) sizeof addr;
    if ((res = ::recvfrom(get_fd(), buffer, size, flags, addr, &len)) < 0) {
        PLOG(Severity::DEBUG, SOCKET_WRITE_FAILED, fd.get_value(), strerror(errno));
    }

    return res;
}

int Socket::_nonblockFD()
{
    try {
        setFSFlags(O_NONBLOCK);
    } catch (Exception &e) {
        return -1;
    }
    return 0;
}

int Socket::_blockFD()
{
    try {
        unsetFSFlags(O_NONBLOCK);
    } catch (Exception &e) {
        return -1;
    }
    return 0;
}

void Socket::close()
{
    CALL_FUNCTION;
    if (isOpen()) {
        if (::close(fd.get_value()) < 0) {
            /*
             * Closing a file descriptor more than once - even when error occures- is
             * not a wise manner.
             *
             * Due to this bahavior ,which explained in close(2)
             * manual page, fd field will be set to zero meaning even though close
             * syscall failed we assume successful closing.
             */
            PLOG(Severity::DEBUG, SOCKET_CLOSE_FAILED, fd.get_value(), strerror(errno));
        } else {
            PLOG(Severity::DEBUG, SOCKET_CLOSE_SUCCESS, fd.get_value());
        }
        fd = 0;
    }
    EXIT_FUNCTION;
}

void Socket::set_protocol(const int protocol)
{
    if (isOpen()) {
        PLOG(Severity::DEBUG, SOCKET_CANNOT_SET_PROTOCOL, fd.get_value());
    } else {
        this->protocol = protocol;
    }
}
void Socket::set_fd(const int fd)
{
    if (isOpen()) {
        PLOG(Severity::DEBUG, SOCKET_CANNOT_SET_FD, this->fd.get_value());
    } else {
        this->fd = fd;
    }
}
void Socket::set_domain(const int domain)
{
    if (isOpen()) {
        PLOG(Severity::DEBUG, SOCKET_CANNOT_SET_DOMAIN, fd.get_value());
    } else {
        this->domain = domain;
    }
}
void Socket::set_type(const int type)
{
    if (isOpen()) {
        PLOG(Severity::DEBUG, SOCKET_CANNOT_SET_TYPE, fd.get_value());
    } else {
        this->type = type;
    }
}
void Socket::setAddr(const Address addr)
{
    CALL_FUNCTION;
    set_ip(addr.first);
    set_port(addr.second);
    EXIT_FUNCTION;
}
void Socket::setAddr(const std::string &path)
{
    CALL_FUNCTION;
    set_unixAddr(path);
    EXIT_FUNCTION;
}

Address Socket::getAddr() const
{
    switch (get_domain()) {
    case SockDom::domain::UNIX:
        return {unixAddr.get_value(), -1};
    case SockDom::domain::IPV4:
    case SockDom::domain::IPV6:
        return {ip.value(), port.get_value()};
    default:
        return {NULL, -1};
    }
}

void Socket::setSockopt(const int optionLevel, const int optionName, const void *optionVal,
                        socklen_t len)
{
    CALL_FUNCTION;
    int status = setsockopt(get_fd(), optionLevel, optionName, optionVal, len);
    if (status < 0) {
        EXIT_FUNCTION_THROW(SOCKET_SET_OPTIONS_FAILED, fd.get_value(), strerror(errno));
    }
    EXIT_FUNCTION;
}

void Socket::getSockopt(const int optionLevel, const int optionName, void *optionVal,
                        socklen_t *len)
{
    CALL_FUNCTION;
    int status = getsockopt(fd.get_value(), optionLevel, optionName, optionVal, len);
    if (status < 0) {
        EXIT_FUNCTION_THROW(SOCKET_GET_OPTIONS_FAILED, fd.get_value(), strerror(errno));
    }
    EXIT_FUNCTION;
}

Address Socket::getPeerName()
{
    switch (get_domain()) {
    case SockDom::domain::UNIX:
        return getUNIXpeer();

    case SockDom::domain::IPV4:
        return getIN4peer();

    case SockDom::domain::IPV6:
        return getIN6peer();
    default:
        EXIT_FUNCTION_THROW(SOCKET_UNDEFINED_DOMAIN);
    }
}

Address Socket::getUNIXpeer()
{
    char ip[2048];
    uint16_t port = -1;

    sockaddr_un addr;
    socklen_t len = (socklen_t) sizeof addr;
    memset(&addr, 0, sizeof addr);
    memset(ip, 0, sizeof *ip);

    if (getpeername(get_fd(), (struct sockaddr *) &addr, &len) < 0) {
        EXIT_FUNCTION_THROW(SOCKET_GET_PEER_FAILED, get_fd(), strerror(errno));
    }

    strncpy(ip, addr.sun_path, sizeof ip - 1);

    return {ip, port};
}

Address Socket::getIN4peer()
{
    char ip[2048];
    uint16_t port = -1;

    sockaddr_in addr;
    socklen_t len = (socklen_t) sizeof addr;

    memset(&addr, 0, sizeof addr);
    memset(ip, 0, sizeof *ip);

    if (getpeername(get_fd(), (struct sockaddr *) &addr, &len) < 0) {
        EXIT_FUNCTION_THROW(SOCKET_GET_PEER_FAILED, get_fd(), strerror(errno));
    }

    inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof ip);
    port = ntohs(addr.sin_port);

    return {ip, port};
}

Address Socket::getIN6peer()
{
    char ip[2048];
    uint16_t port = -1;

    sockaddr_in6 addr;
    socklen_t len = (socklen_t) sizeof addr;

    memset(&addr, 0, sizeof addr);
    memset(ip, 0, sizeof *ip);

    if (getpeername(get_fd(), (struct sockaddr *) &addr, &len) < 0) {
        EXIT_FUNCTION_THROW(SOCKET_GET_PEER_FAILED, get_fd(), strerror(errno));
    }

    inet_ntop(AF_INET6, &addr.sin6_addr, ip, sizeof ip);
    port = ntohs(addr.sin6_port);

    return {ip, port};
}

void Socket::setFSFlags(const int bitmask)
{
    CALL_FUNCTION;
    try {
        setFlags(F_SETFL, getFSFlags() | bitmask);
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }
    EXIT_FUNCTION;
}

void Socket::unsetFSFlags(const int bitmask)
{
    CALL_FUNCTION;
    try {
        setFlags(F_SETFL, getFSFlags() & ~bitmask);
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }
    EXIT_FUNCTION;
}

void Socket::setFDFlags(const int bitmask)
{
    CALL_FUNCTION;
    try {
        setFlags(F_SETFD, getFDFlags() | bitmask);
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }
    EXIT_FUNCTION;
}

void Socket::unsetFDFlags(const int bitmask)
{
    CALL_FUNCTION;
    try {
        setFlags(F_SETFD, getFDFlags() & ~bitmask);
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }
    EXIT_FUNCTION;
}

void Socket::setFlags(const int type, const int bitmask)
{
    CALL_FUNCTION;
    if (fcntl(fd.get_value(), type, bitmask) < 0) {
        EXIT_FUNCTION_THROW(SOCKET_FCNTL_SET_FAILED, fd.get_value(), strerror(errno));
    }
    EXIT_FUNCTION;
}

const int Socket::getFDFlags()
{
    CALL_FUNCTION;
    try {
        EXIT_FUNCTION_RETURN(getFlags(F_GETFD));
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }
}

const int Socket::getFSFlags()
{
    CALL_FUNCTION;
    try {
        EXIT_FUNCTION_RETURN(getFlags(F_GETFL));
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }
}

int Socket::getFlags(const int type)
{
    CALL_FUNCTION;
    int flags = fcntl(fd.get_value(), type);
    if (flags < 0) {
        EXIT_FUNCTION_THROW(SOCKET_FCNTL_GET_FAILED, fd.get_value(), strerror(errno));
    }
    EXIT_FUNCTION_RETURN(flags);
}

void Socket::checkTimeout(const int timeout, const uint32_t flags)
{
    CALL_FUNCTION;

    struct epoll_event epollEvent, epollEventList[1];
    int epollFD, readyFD;

    /* parameters for getSockopt.
     */
    int val;
    socklen_t lon = sizeof(int);

    epollEvent.events = flags;
    epollEvent.data.fd = get_fd();

    if ((epollFD = epoll_create1(EPOLL_CLOEXEC)) < 0) {
        EXIT_FUNCTION_THROW(SOCKET_EPOLL_CREATION_FAILED, get_fd(), strerror(errno));
    }

    if (epoll_ctl(epollFD, EPOLL_CTL_ADD, get_fd(), &epollEvent) < 0) {
        ::close(epollFD);
        EXIT_FUNCTION_THROW(SOCKET_EPOLL_CANNOT_ADD, get_fd(), epollFD, strerror(errno));
    }

    readyFD = epoll_wait(epollFD, epollEventList, 1, timeout);
    ::close(epollFD);

    if (readyFD == 0) {
        EXIT_FUNCTION_THROW(SOCKET_TIMEOUT_EXPIRED, fd.get_value());
    } else if (readyFD == -1) {
        EXIT_FUNCTION_THROW(SOCKET_EPOLL_WAIT_FAILED, get_fd(), epollFD, strerror(errno));
    }

    try {
        /*
         * This call will determine if the opartion
         * was successful ,in timeout period, or not.
         */
        getSockopt(SOL_SOCKET, SO_ERROR, (void *) &val, &lon);
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }

    if (val) {
        EXIT_FUNCTION_THROW(SOCKET_OPERATION_FAILED, fd.get_value(), strerror(val));
    }

    PLOG(Severity::DEBUG, SOCKET_OPERATION_SUCCESS, fd.get_value());
    EXIT_FUNCTION;
}

void Socket::shutdown(const int how)
{
    CALL_FUNCTION;
    int status = ::shutdown(get_fd(), how);
    if (status < 0) {
        EXIT_FUNCTION_THROW(SOCKET_SHUTDOWN_FAILED, fd.get_value(), strerror(errno));
    }
    EXIT_FUNCTION;
}
void Socket::set_unixAddr(const std::string &path)
{
    this->unixAddr = path;
}

void Socket::set_ip(const std::string &ip)
{
    this->ip.setAddress(ip);
}

void Socket::set_port(const unsigned int port)
{
    this->port = port;
}

bool Socket::isOpen() const
{
    return (fd.get_value() > 0);
}

int Socket::get_fd() const
{
    return fd.get_value();
}

int Socket::get_protocol() const
{
    return protocol.get_value();
}
int Socket::get_domain() const
{
    return domain.get_value();
}
int Socket::get_type() const
{
    return type.get_value();
}
std::string Socket::get_ip() const
{
    return ip.value();
}
unsigned int Socket::get_port() const
{
    return port.get_value();
}
std::string Socket::get_unixAddr() const
{
    return unixAddr.get_value();
}

} // namespace net
} // namespace ipc
