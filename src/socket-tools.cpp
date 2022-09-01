#include "socket-tools.hpp"

namespace ipc
{
namespace net
{
sockaddr_un SockTools::convertAddrToNativeUNIX(const char *path)
{
    sockaddr_un addr;
    memset(&addr, 0, sizeof addr);

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

    return addr;
}

sockaddr_in SockTools::convertAddrToNativeIP4(Address addr)
{
    sockaddr_in addr_in;
    memset(&addr_in, 0, sizeof addr_in);

    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(addr.second);
    inet_pton(AF_INET, addr.first.c_str(), (void *) &addr_in.sin_addr);
    return addr_in;
}

sockaddr_in6 SockTools::convertAddrToNativeIP6(Address addr)
{
    sockaddr_in6 addr_in6;
    memset(&addr_in6, 0, sizeof addr_in6);

    addr_in6.sin6_family = AF_INET6;
    addr_in6.sin6_port = htons(addr.second);
    inet_pton(AF_INET6, addr.first.c_str(), (void *) &addr_in6.sin6_addr);

    return addr_in6;
}

std::string convertNativeToAddr(sockaddr_un addr)
{
    char path[128];
    memset(path, 0, sizeof path);

    strncpy(path, addr.sun_path, (sizeof path) - 1);

    return std::string(path);
}

Address convertNativeToAddr(sockaddr_in addr)
{
    char ip[2048];
    uint16_t port = -1;

    memset(&addr, 0, sizeof addr);
    memset(ip, 0, sizeof *ip);

    inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof ip);
    port = ntohs(addr.sin_port);

    return {ip, port};
}

Address convertNativeToAddr(sockaddr_in6 addr)
{
    char ip[2048];
    uint16_t port = -1;

    memset(&addr, 0, sizeof addr);
    memset(ip, 0, sizeof *ip);

    inet_ntop(AF_INET6, &addr.sin6_addr, ip, sizeof ip);
    port = ntohs(addr.sin6_port);

    return {ip, port};
}

} // namespace net
} // namespace ipc
