#include "../headers/Socket.hpp"

int     Socket::getFd()
{
    return fd;
}

Socket::Socket(string port)
{   
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, port.c_str(), &hints, &ai) != 0)
    {
        std::cerr << "getaddrinfo fails" << "\n";
        std::exit(1);
    }

    fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (fd == -1)
    {
        std::cerr << "socket fails" << "\n";
        std::exit(1);
    }

    // kernel keeps port on a waiting mode even if closed so..
    // the systemCall bellow sets a flag to 1 (opt = 1) in the socket layer to be reused.
    int opt = 1;
    //             fd, socketlayer, theFLAG toMOdify, 1  , bytes to go from userspace to kernel space
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR     ,&opt  , sizeof(opt)) < 0)
    {
        std::exit(1);
    }
    if (bind(fd, ai->ai_addr, ai->ai_addrlen))
    {
        std::cerr << "bind fails" << "\n";
        std::exit(1);
    }

    delete ai;

    if (listen(fd, 10))
    {
        std::cerr << "listen fails" << "\n";
        std::exit(1);
    }


    
}