#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "init.hpp"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

using namespace std;

class Socket
{
    private:
        int     fd;
        struct  addrinfo hints, *ai;

    public:
        Socket();
        
        int getFd();
};

#endif