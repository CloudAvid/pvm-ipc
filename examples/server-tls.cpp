#include "server-tls.hpp"

const char *msg = "Server is here\n";
std::string ip = "0.0.0.0";
int port = 7777, timeout = 0;

#undef pproxy

#if pproxy
#define CAFILE   "/etc/ssl/certs/ca-certificates.crt"
#define CERTFILE "/etc/pproxy/certs/PVM-cert.pem"
#define KEYFILE  "/etc/pproxy/certs/PVM-key.pem"
#else
#define CAFILE   "/etc/ssl/certs/ca-certificates.crt"
#define CRLFILE  "/home/manage/pvm/pvm-ipc/gnutls/x509/crl.pem"
#define CERTFILE "/home/manage/pvm/pvm-ipc/gnutls/x509/cert.pem"
#define KEYFILE  "/home/manage/pvm/pvm-ipc/gnutls/x509/key.pem"
#endif

void thrd(TLSServer *server)
{
    char buff[2000];

    try {
        server->handshake();
    } catch (Exception &e) {
        server->close();
    }

    while (true) {
        int ret = 0;
        ret = server->write(msg, strlen(msg));
        if (ret <= 0) {
            cout << GNUTLS_ERROR_DESC(ret) << endl;
            break;
        }
        ret = server->read(buff, 1024);
        if (ret <= 0) {
            cout << GNUTLS_ERROR_DESC(ret) << endl;
            break;
        }
        buff[ret + 1] = '\0';
    }

    if (server->isOpen()) {
        server->close();
    }

    delete server;
}

int main(int argc, char **argv)
{
    // enable this for monitoring result.
    PLogger::init("/etc/sball/plogger.conf");

    if (argc > 2) {
        ip = argv[1];
        port = atoi(argv[2]);
    }

    if (argc > 3) {
        timeout = atoi(argv[3]);
    }

    TLSServer srvr;
    X509Cert *certificate = new X509Cert;

    try {
        certificate->setCAFile(CERTFILE);
        certificate->setCertFile(CERTFILE);
        certificate->setKeyFile(KEYFILE);
    } catch (Exception &e) {
        cout << e.what() << endl;
    }

    Address addr = {ip, port};
    srvr.setAddr(addr);
    srvr.set_cert(certificate);
    try {
        srvr.bind();
        srvr.listen();
    } catch (Exception &e) {
        cout << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    while (1) {
        TLSServer *server = new TLSServer;
        try {
            srvr.accept(*server);
            thread(thrd, server).detach();
        } catch (Exception &e) {
            cout << e.what() << endl;
            continue;
        }
    }
    return 0;
}
#
