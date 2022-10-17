/**
 * \file socket-struct.hpp
 * Socket Structures.
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

#include "macros.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <gcrypt.h>
#include <gnutls/gnutls.h>
#include <netinet/in.h>
#include <poll.h>
#include <pparam/sparam.hpp>
#include <pparam/xparam.hpp>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>

namespace ipc::net
{
typedef std::pair<std::string, int32_t> Address;

/* ----------------- Socket Common Structures ----------------- */

/**
 * @class SockDom.
 * @brief Socket's domains.
 */
class SockDom
{
public:
    enum domain
    {
        UNIX,
        IPV4,
        IPV6,
        MAX
    };
    static const std::string typeString[MAX];
};

/**
 * @class SockType.
 * @brief Socket's types.
 */
class SockType
{
public:
    enum type
    {
        TCP,
        UDP,
        MAX
    };
    static const std::string typeString[MAX];
};

/* ----------------- Socket TLS Structures ----------------- */

/**
 * @class X509Cert.
 * @brief PVM x509 Certificate structure.
 */
class X509Cert
{
public:
    X509Cert();

    ~X509Cert();

    void setCertFile(const std::string &certFile);

    void setCAFile(const std::string &caFile);

    void setKeyFile(const std::string &keyFile);

    void setCrlFile(const std::string &crlFile);

    void setCADir(const std::string &dir);

    void set_cred(const gnutls_certificate_credentials_t cred);
    void set_priority(const gnutls_priority_t priority);

    gnutls_certificate_credentials_t get_cred() const;
    gnutls_priority_t get_priority() const;

private:
    gnutls_certificate_credentials_t cred;

    gnutls_priority_t priority;

    std::string certFile;

    std::string certPath;

    std::string crlFile;

    std::string keyFile;

    std::string caFile;
};

} // namespace ipc::net
