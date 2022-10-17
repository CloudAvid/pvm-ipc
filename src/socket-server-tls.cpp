#include "socket-server-tls.hpp"

#include "plogger.hpp"

namespace ipc::net
{
/* ----------------------- TLS Server Class -------------------------- */

TLSServer::TLSServer()
{
    // TODO Remove this
    signal(SIGPIPE, SIG_IGN);

    session = NULL;
    set_hsTimeout(GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);
}

void TLSServer::initSession()
{
    gnutls_init(&session, GNUTLS_SERVER);
}

void TLSServer::close()
{
    Socket::close();
}

void TLSServer::handshake(int fd)
{
    CALL_FUNCTION;

    int result = 0;

    if (session == NULL) {
        gnutls_init(&session, GNUTLS_SERVER);
    }

    fd = (fd ? fd : get_fd());

    gnutls_priority_set(session, cert->get_priority());
    gnutls_credentials_set(session, GNUTLS_CRD_CERTIFICATE, cert->get_cred());

    // gnutls_session_set_verify_cert(session, NULL, 0);

    gnutls_certificate_server_set_request(session, GNUTLS_CERT_REQUEST);
    gnutls_handshake_set_timeout(session, hsTimeout);
    gnutls_certificate_send_x509_rdn_sequence(session, 0);
    gnutls_transport_set_int(session, fd);

    LOOP_CHECK(result, gnutls_handshake(session));

    if (result != GNUTLS_E_SUCCESS) {
        gnutls_deinit(session);
        THROW_EXCEPTION(SOCKET_TLS_HANDSHAKE_FAILURE, fd, GNUTLS_ERROR_DESC(result));
    }

    DEBUG(SOCKET_TLS_HANDSHAKE_SUCCESS, fd);

    EXIT_FUNCTION;
}

void TLSServer::accept(TLSServer &srvr)
{
    CALL_FUNCTION;

    try {
        Server::accept(srvr);
        srvr.set_cert(cert);
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }

    EXIT_FUNCTION;
}

const ssize_t TLSServer::write(const char *buffer, const size_t size)
{
    int ret = 0;
    LOOP_CHECK(ret, gnutls_record_send(session, buffer, size));
    return ret;
}

void TLSServer::fullWrite(const char *buffer, const size_t size)
{
    int len = size;

    while (len > 0) {
        ssize_t result = write(buffer + (size - len), len);
        if (result == 0) {
            THROW_EXCEPTION(SOCKET_TLS_WRITE_PEER_CLOSED);
        }

        if (result < 0) {
            THROW_EXCEPTION(SOCKET_TLS_WRITE_FAILURE, GNUTLS_ERROR_DESC(result));
        }

        len -= result;
    }
}

const ssize_t TLSServer::read(char *buffer, const size_t size)
{
    int ret = 0;
    LOOP_CHECK(ret, gnutls_record_recv(session, buffer, size));
    return ret;
}

void TLSServer::bye()
{
    if (session != NULL)
        gnutls_bye(session, GNUTLS_SHUT_WR);
}

void TLSServer::set_cert(X509Cert *cert)
{
    this->cert = cert;
}

void TLSServer::set_session(gnutls_session_t session)
{
    this->session = session;
}

void TLSServer::set_hsTimeout(const int hsTimeout)
{
    this->hsTimeout = hsTimeout;
}

int TLSServer::get_hsTimeout() const
{
    return hsTimeout;
}

} // namespace ipc::net
