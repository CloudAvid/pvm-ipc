#include "socket-client-tls.hpp"

#include "plogger.hpp"

namespace ipc::net
{
/* ----------------------- TLS Client Class -------------------------- */

TLSClient::TLSClient()
{
    session = NULL;
    set_hsTimeout(GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);
}

void TLSClient::close()
{
    Socket::close();
}

void TLSClient::handshake()
{
    CALL_FUNCTION;

    int result = 0;

    if (session == NULL) {
        gnutls_init(&session, GNUTLS_CLIENT);
    }

    gnutls_priority_set(session, cert->get_priority());
    gnutls_credentials_set(session, GNUTLS_CRD_CERTIFICATE, cert->get_cred());

    // gnutls_session_set_verify_cert(session, NULL, 0);

    gnutls_handshake_set_timeout(session, hsTimeout);
    gnutls_transport_set_int(session, get_fd());

    LOOP_CHECK(result, gnutls_handshake(session));

    if (result != GNUTLS_E_SUCCESS) {
        gnutls_deinit(session);
        THROW_EXCEPTION(SOCKET_TLS_HANDSHAKE_FAILURE, get_fd(), GNUTLS_ERROR_DESC(result));
    }

    DEBUG(SOCKET_TLS_HANDSHAKE_SUCCESS, get_fd());

    EXIT_FUNCTION;
}

void TLSClient::connect(const int timeout)
{
    CALL_FUNCTION;

    try {
        Client::connect(timeout);
    } catch (Exception &e) {
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }

    try {
        handshake();
    } catch (Exception &e) {
        close();
        EXIT_FUNCTION_THROW_EXCEPTION(e);
    }

    EXIT_FUNCTION;
}

const ssize_t TLSClient::write(const char *buffer, const size_t size)
{
    int ret = 0;
    LOOP_CHECK(ret, gnutls_record_send(session, buffer, size));
    return ret;
}

void TLSClient::fullWrite(const char *buffer, const size_t size)
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

const ssize_t TLSClient::read(char *buffer, const size_t size)
{
    int ret = 0;
    LOOP_CHECK(ret, gnutls_record_recv(session, buffer, size));
    return ret;
}

void TLSClient::bye()
{
    gnutls_bye(session, GNUTLS_SHUT_WR);
}

void TLSClient::set_cert(X509Cert *cert)
{
    this->cert = cert;
}

void TLSClient::set_hsTimeout(const int hsTimeout)
{
    this->hsTimeout = hsTimeout;
}

int TLSClient::get_hsTimeout() const
{
    return hsTimeout;
}
} // namespace ipc::net
