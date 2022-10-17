/**
 * \file plogger.hpp
 * Implementations that is required by pvm-ipc.
 *
 * Copyright 2012..2022 Cloud Avid. (www.cloudavid.com)
 * \author Ali Nasrolahi (nasrollahi@cloudavid.com)
 *
 * pvm-ipc is free software: you can redistribute it and/or modify it
 * under the terms of GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the license, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with pvm-ipc. If not, see <http://www.gnu.org/licenses/>
 */
#pragma once

#include <plogger/plogger.hpp>
#include <plogger/translator.hpp>

using plogger::PLogger;
using plogger::Severity;

/* Name of IPC module */
#define IPC_MODULE "pvm-ipc"

namespace ipc
{
enum ELogID
{
    /* Socket class */
    SOCKET_UNDEFINED_DOMAIN = PLOGGER_FIRST_LOGID,
    SOCKET_UNDEFINED_TYPE,
    SOCKET_CREATION_SUCCESS,
    SOCKET_CREATION_FAILED,
    SOCKET_CREATION_FAILED_IS_OPEN,
    SOCKET_WRITE_FAILED,
    SOCKET_FULL_WRITE_FAILED,
    SOCKET_READ_FAILED,
    SOCKET_FULL_READ_FAILED,
    SOCKET_CLOSE_SUCCESS,
    SOCKET_CLOSE_FAILED,
    SOCKET_GET_OPTIONS_FAILED,
    SOCKET_SET_OPTIONS_FAILED,
    SOCKET_CANNOT_SET_PROTOCOL,
    SOCKET_CANNOT_SET_FD,
    SOCKET_CANNOT_SET_TYPE,
    SOCKET_CANNOT_SET_DOMAIN,
    SOCKET_FCNTL_GET_FAILED,
    SOCKET_FCNTL_SET_FAILED,
    SOCKET_FCNTL_BLOCK_FAILED,
    SOCKET_FCNTL_NONBLOCK_FAILED,
    SOCKET_FCNTL_UNSET_FAILED,
    SOCKET_SHUTDOWN_FAILED,
    SOCKET_SHUTDOWN_SUCCESS,
    SOCKET_OPERATION_SUCCESS,
    SOCKET_OPERATION_FAILED,
    SOCKET_GET_PEER_FAILED,
    SOCKET_EPOLL_CREATION_FAILED,
    SOCKET_EPOLL_CANNOT_ADD,
    SOCKET_EPOLL_WAIT_FAILED,

    /* Client Socket class */
    SOCKET_CONNECTION_FAILED,
    SOCKET_CONNECTION_SUCCESS,
    SOCKET_TIMEOUT_SET,
    SOCKET_TIMEOUT_EXPIRED,

    /* Server Socket class */
    SOCKET_LISTEN_SUCCESS,
    SOCKET_LISTEN_FAILED,
    SOCKET_BIND_FAILED,
    SOCKET_BIND_SUCCESS,
    SOCKET_ACCEPT_FAILED,
    SOCKET_ACCEPT_SUCCESS,

    /* TLS Socket */
    SOCKET_TLS_HANDSHAKE_SUCCESS,
    SOCKET_TLS_HANDSHAKE_FAILURE,
    SOCKET_TLS_WRITE_PEER_CLOSED,
    SOCKET_TLS_WRITE_FAILURE,
    SOCKET_TLS_READ_PEER_CLOSED,
    SOCKET_TLS_READ_FAILURE,

    /* Socket Structs */
    SOCKET_SET_CA_FAILED,
    SOCKET_SET_KEY_FAILED,
    SOCKET_SET_CRL_FAILED,
    SOCKET_SET_CA_DIR_FAILED,
};

/**
 * \class Translator
 * IPC logID translator.
 */
class Translator : public plogger::Translator
{
public:
    virtual string getLogFormat(plogger::LogID logID);
    virtual string getModule();
};

/**
 * IPC translator.
 */
extern Translator IPCTranslator;

/**
 * Would be used by PLOG Macro to aquire ipc translator.
 */
Translator *PLoggerTranslator();

} // namespace ipc
