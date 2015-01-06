#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <string>
#include "StreamCommuniCation.h"
#ifndef WIN32
#include <sys/socket.h>
    #include <netinet/in.h>
#else
    #include <winsock2.h>
#endif

