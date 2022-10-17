/**
 * This example creates (NUMS) threads and each
 * one of them connects to address then ->(writes<->read)->close
 * and all over again.
 */

#include "plogger.hpp"
#include "socket-client.hpp"

#include <ctime>
using namespace ipc::net;
using namespace std;

void thrd();

int main(int argc, char **argv);
