#include "client-example.hpp"

const std::string msg = "Client is Here :)";
std::string Addr = "127.0.0.1";
int port = 7777, timeout = 0;

void thrd()
{
    srand((unsigned) time(0));
    long long readBytes = 0;
    int bytes = 0;
    while (1) {
        Client clnt;
        Address addr = {Addr, port};
        clnt.setAddr(addr);
        try {
            /*
             * To use timeout try: clnt.connect(timeout);
             */
            clnt.connect();
        } catch (Exception &e) {
            cout << e.what() << endl;
            clnt.close();
            break;
        }
        char buff[1028];
        while (true) {
            /*
             * To use timeout try: clnt.write(msg, timeout);
             */
            bytes = clnt.write(msg);
            if (bytes < 0) {
                bytes = 0;
                break;
            }
            /*
             * To use timeout try: clnt.read(msg, timeout);
             */
            bytes = clnt.read(buff, 1024);
            if (bytes < 0) {
                bytes = 0;
                break;
            }
            readBytes += bytes;
            if (readBytes >= 0x800) {
                readBytes = 0;
                if (! (rand() % 100)) {
                    break;
                }
            }
        }
        clnt.close();
    }
}

int main(int argc, char **argv)
{
    // enable this for displaying logs.
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
