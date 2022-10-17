/**
 * \file socket-server-tls.hpp
 * TLS Supoort for Server Socket
 *
 * \author Ali Nasrolahi (nasrollahi@acloudavid.com)
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
#include "socket-server.hpp"

namespace ipc::net
{
/**
 * @class TLSServer.
 * @brief handles Server-sided TLS communications.
 */
class TLSServer : public Server
{
public:
    /**
     * @brief TLS Constructor.
     */
    TLSServer();

    /**
     * @brief Initialize gnutls session.
     */
    void initSession();

    /**
     * @brief Accepts connections.
     * @param srvr will be filled with accepted socket.
     */
    void accept(TLSServer &srvr);

    /**
     * @brief perfoms gnutls_handshake with peer.
     * @param fd socket of accepted peer.
     */
    void handshake(int fd = 0);

    /**
     * @brief Closes socket.
     */
    void close() override;

    /**
     * @brief Reads form socket and fills buffer.
     * @param buffer value-result of read data.
     * @param size size of buffer.
     *
     * @return read bytes.
     */
    const ssize_t read(char *buffer, const size_t size) override;

    /**
     * @brief Writes to socket.
     * @param buffer data which need to be sent.
     * @param size size of buffer.
     *
     * @return wrriten bytes.
     */
    const ssize_t write(const char *buffer, const size_t size) override;

    /**
     * @brief Writes buffer fully.
     * @param buffer data which need to be sent.
     * @param size size of buffer.
     *
     */
    void fullWrite(const char *buffer, const size_t size) override;

    /**
     * @brief shutdowns R/W in socket.
     */
    void bye();

    void set_cert(X509Cert *cert);
    void set_session(gnutls_session_t session);
    void set_hsTimeout(const int hsTimeout);

    int get_hsTimeout() const;

private:
    /**
     * @brief Gnutls session.
     */
    gnutls_session_t session;

    /**
     * @brief X509 certification.
     */
    X509Cert *cert;

    /**
     * @brief handshake timeout.
     */
    int hsTimeout;
};

} // namespace ipc::net
