#include "socket-server.hpp"

#include "plogger.hpp"

using namespace plogger;

namespace ipc
{
namespace net
{

Server::Server(const string &pname) : Socket(pname), backlog("backlog", 0, -1)
{
}

Server::Server(const SockDom::domain domain, const SockType::type type, const string &pname) :
    Socket(domain, type, pname),
    backlog("backlog", 0, -1)
{
}

Server::Server(const Server &srvr) : Socket(srvr), backlog("backlog", 0, -1)
{
    set_backlog(srvr.get_backlog());
}

void Server::listen()
{
    CALL_FUNCTION;
    if (::listen(get_fd(), get_backlog()) < 0) {
        EXIT_FUNCTION_THROW(SOCKET_LISTEN_FAILED, get_fd(), strerror(errno));
    }
    PLOG(Severity::DEBUG, SOCKET_LISTEN_SUCCESS, getAddr().first.c_str(), getAddr().second,
         get_fd());

    EXIT_FUNCTION;
}

int Server::accept()
{
    CALL_FUNCTION;
    struct sockaddr_in clientAddr;
    int addrlen = sizeof(clientAddr);
    const int newFD = ::accept4(get_fd(), (struct sockaddr *) &clientAddr, (socklen_t *) &addrlen,
                                SOCK_CLOEXEC);

    if (newFD < 0) {
        EXIT_FUNCTION_THROW(SOCKET_ACCEPT_FAILED, get_fd(), strerror(errno));
    }

    PLOG(Severity::DEBUG, SOCKET_ACCEPT_SUCCESS, get_fd(), newFD);
    EXIT_FUNCTION_RETURN(newFD);
}

void Server::accept(Server &acceptedSocket)
{
    CALL_FUNCTION;

    int newFD;
    try {
        newFD = accept();
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }

    acceptedSocket.set_protocol(get_protocol());
    acceptedSocket.set_domain(get_domain());
    acceptedSocket.set_type(get_type());
    acceptedSocket.set_ip(get_ip());
    acceptedSocket.set_port(get_port());
    acceptedSocket.set_unixAddr(get_unixAddr());
    acceptedSocket.set_fd(newFD);

    PLOG(Severity::DEBUG, SOCKET_ACCEPT_SUCCESS, get_fd(), newFD);
    EXIT_FUNCTION;
}

void Server::bind()
{
    CALL_FUNCTION;
    try {
        create_fd();
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }

    try {
        /*
         * Seting REUSEADDR flag
         * which gives us the ability to resuse address instantly
         */
        int optionVal = 1;
        setSockopt(SOL_SOCKET, SO_REUSEADDR, (void *) &optionVal, sizeof(optionVal));
    } catch (Exception &e) {
        /*
         * On failure we'll still move on.
         */
    }

    try {
        switch (get_domain()) {
        case SockDom::domain::UNIX:
            bindToUNIX();
            break;
        case SockDom::domain::IPV4:
            bindToIN4();
            break;
        case SockDom::domain::IPV6:
            bindToIN6();
            break;
        default:
            EXIT_FUNCTION_THROW(SOCKET_UNDEFINED_DOMAIN);
        }
    } catch (Exception &e) {
        close();
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }

    PLOG(Severity::DEBUG, SOCKET_BIND_SUCCESS, getAddr().first.c_str(), getAddr().second, get_fd());
    EXIT_FUNCTION;
}
void Server::bindToUNIX()
{
    CALL_FUNCTION;

    unlink(getAddr().first.c_str());

    if (::bind(get_fd(), (struct sockaddr *) &nativeUNIXaddr, sizeof(struct sockaddr_un)) < 0) {
        EXIT_FUNCTION_THROW(SOCKET_BIND_FAILED, get_fd(), strerror(errno));
    }

    EXIT_FUNCTION;
}

void Server::bindToIN4()
{
    CALL_FUNCTION;

    if (::bind(get_fd(), (struct sockaddr *) &nativeIP4addr, sizeof(struct sockaddr_in)) < 0) {
        EXIT_FUNCTION_THROW(SOCKET_BIND_FAILED, get_fd(), strerror(errno));
    }

    EXIT_FUNCTION;
}

void Server::bindToIN6()
{
    CALL_FUNCTION;

    if (::bind(get_fd(), (struct sockaddr *) &nativeIP6addr, sizeof(struct sockaddr_in6)) < 0) {
        EXIT_FUNCTION_THROW(SOCKET_BIND_FAILED, get_fd(), strerror(errno));
    }

    EXIT_FUNCTION;
}

int Server::get_backlog() const
{
    return backlog.get_value();
}
void Server::set_backlog(const int backlog)
{
    this->backlog = backlog;
}

} // namespace net
} // namespace ipc
