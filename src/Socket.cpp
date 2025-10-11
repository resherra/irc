#include "../headers/Socket.hpp"

int     Socket::getFd()
{
    return fd;
}

Socket::Socket(string port)
{   
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, port.c_str(), &hints, &ai) != 0)
    {
        cerr << "getaddrinfo fails" << "\n";
        exit(1);
    }

    fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (fd == -1)
    {
        cerr << "socket fails" << "\n";
        exit(1);
    }

    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        exit(1);
    }
    if (bind(fd, ai->ai_addr, ai->ai_addrlen))
    {
        cerr << "bind fails" << "\n";
        exit(1);
    }
    delete ai;
    if (listen(fd, 10))
    {
        cerr << "listen fails" << "\n";
        exit(1);
    }    
}