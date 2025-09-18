#include "../headers/Socket.hpp"

int     Socket::getFd()
{
    return fd;
}

Socket::Socket()
{   
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, PORT, &hints, &ai) != 0)
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