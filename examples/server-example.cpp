#include "server-example.hpp"

const string msg = "Server is here";
std::string ip = "127.0.0.1";
int port = 7777, timeout = 0;

void thrd(Server *server)
{
    char buffer[2000];
    while (true) {
        /*
         * To use timeout try: server->write(msg, timeout);
         */
        if (server->write(msg) < 0)
            break;
        /*
         * To use timeout try: server->read(msg, timeout);
         */
        if (server->read(buffer, 1024) <= 0)
            break;
    }
    server->close();
    delete server;
}
int main(int argc, char **argv)
{
    // enable this for monitoring result.
    PLogger::init(PLOGGER_CONF_PATH);

    if (argc > 2) {
        ip = argv[1];
        port = atoi(argv[2]);
    }

    if (argc > 3) {
        timeout = atoi(argv[3]);
    }

    Server srvr;
    Address adrr = {ip, port};
    srvr.setAddr(adrr);
    try {
        srvr.bind();
        srvr.listen();
    } catch (Exception &e) {
        cout << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    while (1) {
        Server *server = new Server;
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
