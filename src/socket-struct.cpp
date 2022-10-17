#include "socket-struct.hpp"

#include "plogger.hpp"

namespace ipc::net
{
/* ----------------- Socket Common Structures ----------------- */

const std::string SockDom::typeString[SockDom::MAX] = {"UNIX", "IPV4", "IPV6"};

const std::string SockType::typeString[SockType::MAX] = {"TCP", "UDP"};

/* ----------------- Socket TLS Structures ----------------- */

/* Implementation of "X509Cert" class */

X509Cert::X509Cert()
{
    gnutls_certificate_allocate_credentials(&cred);
    gnutls_priority_init(&priority, NULL, NULL);
}

X509Cert::~X509Cert()
{
    gnutls_certificate_free_credentials(cred);
    gnutls_priority_deinit(priority);
}

void X509Cert::setCertFile(const std::string &certFile)
{
    CALL_FUNCTION;

    this->certFile = certFile;

    EXIT_FUNCTION;
}

void X509Cert::setCAFile(const std::string &caFile)
{
    CALL_FUNCTION;
    int result = gnutls_certificate_set_x509_trust_file(cred, caFile.c_str(), GNUTLS_X509_FMT_PEM);
    if (result < 0) {
        THROW_EXCEPTION(SOCKET_SET_CA_FAILED, GNUTLS_ERROR_DESC(result));
    }

    this->caFile = caFile;

    EXIT_FUNCTION;
}

void X509Cert::setKeyFile(const std::string &keyFile)
{
    CALL_FUNCTION;
    int result = gnutls_certificate_set_x509_key_file(cred, certFile.c_str(), keyFile.c_str(),
                                                      GNUTLS_X509_FMT_PEM);

    if (result < 0) {
        THROW_EXCEPTION(SOCKET_SET_KEY_FAILED, GNUTLS_ERROR_DESC(result));
    }

    this->keyFile = keyFile;

    EXIT_FUNCTION;
}

void X509Cert::setCrlFile(const std::string &crlFile)
{
    CALL_FUNCTION;

    int result = gnutls_certificate_set_x509_crl_file(cred, crlFile.c_str(), GNUTLS_X509_FMT_PEM);

    if (result < 0) {
        THROW_EXCEPTION(SOCKET_SET_CRL_FAILED, GNUTLS_ERROR_DESC(result));
    }

    this->crlFile = crlFile;

    EXIT_FUNCTION;
}

void X509Cert::setCADir(const std::string &dir)
{
    CALL_FUNCTION;

    int result = gnutls_certificate_set_x509_trust_dir(cred, dir.c_str(), GNUTLS_X509_FMT_PEM);

    if (result < 0) {
        THROW_EXCEPTION(SOCKET_SET_CA_DIR_FAILED, GNUTLS_ERROR_DESC(result));
    }

    this->caFile = dir;

    EXIT_FUNCTION;
}

void X509Cert::set_cred(const gnutls_certificate_credentials_t cred)
{
    this->cred = cred;
}

void X509Cert::set_priority(const gnutls_priority_t priority)
{
    this->priority = priority;
}

gnutls_certificate_credentials_t X509Cert::get_cred() const
{
    return cred;
}

gnutls_priority_t X509Cert::get_priority() const
{
    return priority;
}

} // namespace ipc::net
