#include "plogger.hpp"

namespace ipc
{
Translator IPCTranslator;

/* Implementation of Translator Class.
 */
string Translator::getLogFormat(plogger::LogID logID)
{
    switch (logID) {
    case SOCKET_UNDEFINED_DOMAIN:
        return "Undefined socket domain";
    case SOCKET_UNDEFINED_TYPE:
        return "Undefined socket type";
    case SOCKET_CREATION_SUCCESS:
        return "Socket Created, and Socket FD is: %d";
    case SOCKET_CREATION_FAILED:
        return "Socket Creation failed with Error: %s";
    case SOCKET_CREATION_FAILED_IS_OPEN:
        return "Socket Creation failed. FD is still open. FD is: %d";
    case SOCKET_WRITE_FAILED:
        return "Socket write failed, FD is %d and Error is %s";
    case SOCKET_FULL_WRITE_FAILED:
        return "Socket could not write data fully, FD is %d and Error is %s";
    case SOCKET_READ_FAILED:
        return "Socket read failed, FD is %d and Error is %s";
    case SOCKET_FULL_READ_FAILED:
        return "Socket couldn't read data fully, FD is %d and Error is %s";
    case SOCKET_CLOSE_SUCCESS:
        return "Socket closed %d successfully";
    case SOCKET_CLOSE_FAILED:
        return "Socket failed to close FD: %d and Error is: %s";
    case SOCKET_CANNOT_SET_FD:
        return "Socket cannot set FD while %d fd is open";
    case SOCKET_CANNOT_SET_PROTOCOL:
        return "Socket cannot set protocol while %d fd is open";
    case SOCKET_CANNOT_SET_DOMAIN:
        return "Socket cannot set domain while %d fd is open";
    case SOCKET_CANNOT_SET_TYPE:
        return "Socket cannot set type while %d fd is open";
    case SOCKET_SET_OPTIONS_FAILED:
        return "Setting socket's options failed for FD: %d with Error: %s";
    case SOCKET_GET_OPTIONS_FAILED:
        return "Getting socket's options failed for FD: %d with Error: %s";
    case SOCKET_FCNTL_GET_FAILED:
        return "Couldn't get fcntl to get flags for FD: %d with Error: %s";
    case SOCKET_FCNTL_SET_FAILED:
        return "Couldn't get fcntl to set flags for FD: %d with Error: %s";
    case SOCKET_FCNTL_BLOCK_FAILED:
        return "Couldn't block the FD: %d with Error: %s";
    case SOCKET_FCNTL_NONBLOCK_FAILED:
        return "Couldn't nonblock the FD: %d with Error: %s";
    case SOCKET_FCNTL_UNSET_FAILED:
        return "Couldn't get fcntl to unset flags for FD: %d with Error: %s";
    case SOCKET_SHUTDOWN_FAILED:
        return "Couldn't shutdown the socket. FD is %d and Error is %s";
    case SOCKET_SHUTDOWN_SUCCESS:
        return "Socket successfully shutdown, Socket FD is %d";
    case SOCKET_OPERATION_FAILED:
        return "Operation couldn't be established on FD: %d with Error: %s";
    case SOCKET_OPERATION_SUCCESS:
        return "Successful operation established on FD: %d";
    case SOCKET_GET_PEER_FAILED:
        return "Getting peer name failed for Socket %d with Error: %s";
    case SOCKET_EPOLL_CREATION_FAILED:
        return "Epoll creation failed. Socket FD is %d and error is %s";
    case SOCKET_EPOLL_CANNOT_ADD:
        return "Epoll cannot add FD %d. Epoll FD is %d and error is %s";
    case SOCKET_EPOLL_WAIT_FAILED:
        return "Epoll wait method failed for FD: %d. Epoll FD is %d and error is %s";

    /* Client Socket class */
    case SOCKET_CONNECTION_FAILED:
        return "Connection couldn't be established on FD: %d with Error: %s";
    case SOCKET_CONNECTION_SUCCESS:
        return "Successful connection established on FD: %d";
    case SOCKET_TIMEOUT_SET:
        return "Timeout set for connect. Time sec: %d, Time usec: %lld";
    case SOCKET_TIMEOUT_EXPIRED:
        return "Timeout expired. Socket FD is %d";

    /* Server Socket class */
    case SOCKET_LISTEN_SUCCESS:
        return "Socket is listening on IP: %s ,Port: %d. Socket FD is %d";
    case SOCKET_LISTEN_FAILED:
        return "Socket listen failed to listen. Socket FD is %d and Error is %s";
    case SOCKET_BIND_FAILED:
        return "Socket cannot bind. Socket FD is %d and Error is %s";
    case SOCKET_BIND_SUCCESS:
        return "Socket binded to IP: %s ,Port: %d. Socket FD is %d";
    case SOCKET_ACCEPT_FAILED:
        return "Socket cannot accept. Socket FD is %d and Error is %s";
    case SOCKET_ACCEPT_SUCCESS:
        return "Socket %d accepted %d as a new socket.";

    /* TLS Socket Class */
    case SOCKET_TLS_HANDSHAKE_SUCCESS:
        return "Handshake with FD: %i has completed.";
    case SOCKET_TLS_HANDSHAKE_FAILURE:
        return "Handshake with FD: %i failed because: %s";
    case SOCKET_TLS_WRITE_PEER_CLOSED:
        return "TLS peer closed connection while sending message";
    case SOCKET_TLS_WRITE_FAILURE:
        return "Seding message failed because: %s";
    case SOCKET_TLS_READ_PEER_CLOSED:
        return "TLS peer close connection while reciveing meeage.";
    case SOCKET_TLS_READ_FAILURE:
        return "Recieving message failed because: %s";

    /* Socket Structs */
    case SOCKET_SET_CA_FAILED:
        return "Setting CA file failed because: %s";
    case SOCKET_SET_KEY_FAILED:
        return "Setting Key file failed because: %s";
    case SOCKET_SET_CRL_FAILED:
        return "Setting CRL file failed because: %s";
    case SOCKET_SET_CA_DIR_FAILED:
        return "Setting CA directory failed because: %s";

    default:
        return PLOGGER_NONE;
    };
}

string Translator::getModule()
{
    return IPC_MODULE;
}

Translator *PLoggerTranslator()
{
    return &IPCTranslator;
}

} // namespace ipc
