#include "socket-client.hpp"

#include "plogger.hpp"

using namespace plogger;

namespace ipc
{
namespace net
{
Client::Client(const string &pname) : Socket(pname)
{
}
Client::Client(const Client &clnt) : Socket(clnt)
{
}
Client::Client(const SockDom::domain domain, const SockType::type type, const string &pname) :
    Socket(domain, type, pname)
{
}

void Client::connect(const int timeout)
{
    CALL_FUNCTION;

    try {
        create_fd();
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }

    if (timeout > 0) {
        try {
            setFSFlags(O_NONBLOCK);
        } catch (Exception &e) {
            close();
            EXIT_FUNCTION_THROW_EXCEPTION(e);
        }
        PLOG(Severity::DEBUG, SOCKET_TIMEOUT_SET, (timeout / 1000), (timeout % 1000));
    }

    errno = 0;

    switch (get_domain()) {
    case SockDom::domain::UNIX:
        connectToUNIX();
        break;
    case SockDom::domain::IPV4:
        connectToIN4();
        break;
    case SockDom::domain::IPV6:
        connectToIN6();
        break;
    default:
        close();
        EXIT_FUNCTION_THROW(SOCKET_UNDEFINED_DOMAIN);
    }

    if (errno) {
        if (timeout > 0) {
            try {
                if (errno == EWOULDBLOCK || errno == EINPROGRESS) {
                    uint32_t flags = EPOLLIN | EPOLLOUT | EPOLLERR;
                    PLOG(Severity::TRACE, SOCKET_CONNECTION_FAILED, fd.get_value(),
                         "checking for timeout");
                    checkTimeout(timeout, flags);
                    unsetFSFlags(O_NONBLOCK);
                } else {
                    EXIT_FUNCTION_THROW(SOCKET_CONNECTION_FAILED, fd.get_value(), strerror(errno));
                }
            } catch (Exception &e) {
                close();
                EXIT_FUNCTION_THROW_EXCEPTION(e);
            }
        } else {
            close();
            EXIT_FUNCTION_THROW(SOCKET_CONNECTION_FAILED, fd.get_value(), strerror(errno));
        }
    }

    EXIT_FUNCTION;
}

void Client::connectToUNIX()
{
    int len = sizeof nativeUNIXaddr;
    ::connect(get_fd(), (struct sockaddr *) &nativeUNIXaddr, (socklen_t) len);
}

void Client::connectToIN4()
{
    int len = sizeof nativeIP4addr;
    ::connect(get_fd(), (struct sockaddr *) &nativeIP4addr, (socklen_t) len);
}

void Client::connectToIN6()
{
    int len = sizeof nativeIP6addr;
    ::connect(get_fd(), (struct sockaddr *) &nativeIP6addr, (socklen_t) len);
}

} // namespace net
} // namespace ipc
