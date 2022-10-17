#include "client-tls.hpp"

const char *msg = "Client is Here :)\n";
std::string Addr = "127.0.0.1";
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

void thrd()
{
    srand((unsigned) time(0));
    long long readBytes = 0;
    int ret = 0;
    X509Cert *certificate = new X509Cert;
    try {
        certificate->setCAFile(CERTFILE);
        certificate->setCertFile(CERTFILE);
        certificate->setKeyFile(KEYFILE);
    } catch (Exception &e) {
        cout << e.what() << endl;
    }

    while (1) {
        TLSClient clnt;
        Address addr = {Addr, port};
        clnt.setAddr(addr);
        clnt.set_cert(certificate);
        try {
            /*
             * To use timeout try: clnt.connect(timeout);
             */
            clnt.connect();
        } catch (Exception &e) {
            break;
        }
        char buff[4000];
        while (true) {
            ret = clnt.read(buff, 1024);
            if (ret <= 0) {
                cout << GNUTLS_ERROR_DESC(ret) << endl;
                break;
            }

            ret = clnt.write(msg, strlen(msg));
            if (ret <= 0) {
                cout << GNUTLS_ERROR_DESC(ret) << endl;
                break;
            }
            buff[ret + 1] = '\0';
            readBytes += ret;
            if (readBytes >= 0x800) {
                readBytes = 0;
                if (! (rand() % 100)) {
                    break;
                }
            }
        }
        if (clnt.isOpen()) {
            clnt.close();
        }
    }
    delete certificate;
}

int main(int argc, char **argv)
{
    // enable this for monitoring result.
    PLogger::init(PLOGGER_CONF_PATH);

    if (argc > 2) {
        Addr = argv[1];
        port = atoi(argv[2]);
    }
    if (argc > 3) {
        timeout = atoi(argv[3]);
    }

    int nums;
    cout << "NUMS:\n";
    cin >> nums;
    thread th[nums];
    for (int i = 0; i < nums; i++)
        th[i] = thread(thrd);
    for (int i = 0; i < nums; i++)
        th[i].join();
    return 0;
}
