#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>

using namespace std;

class Socket
{
    private:
        int     fd;
        struct  addrinfo hints, *ai;

    public:
        Socket(string);
        
        int getFd();
};

#endif