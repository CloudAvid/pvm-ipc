/**
 * This example receives connection then (read<->write)->close it.
 */
#include "plogger.hpp"
#include "socket-server.hpp"

using namespace std;
using namespace ipc::net;

void thrd(Server *server);
