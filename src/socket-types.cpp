#include "socket-types.hpp"

namespace ipc
{
namespace net
{
const std::string SockDom::typeString[SockDom::MAX] = {"UNIX", "IPV4", "IPV6"};
const std::string SockType::typeString[SockType::MAX] = {"TCP", "UDP"};
} // namespace net
} // namespace ipc
