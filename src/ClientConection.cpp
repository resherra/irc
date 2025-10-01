#include "../headers/Server.hpp"

void Server::createNewClient(int newfd)
{
    std::pair<int, Client> cli(newfd, Client(newfd));
    
    struct pollfd new_fd;
    new_fd.fd = newfd;
    new_fd.events = POLLIN;
    new_fd.revents = 0;

    clients.insert(cli);
    pfds.push_back(new_fd);
    fd_count++;
}

void Server::newClient()
{
    struct sockaddr_storage remoteaddr;
    socklen_t               addrlen;
    int                     client_fd;

    addrlen = sizeof remoteaddr;
    client_fd = accept(sock_fd.getFd(), (struct sockaddr *)&remoteaddr,
                       &addrlen);
    if (client_fd == -1)
        perror("accept");
    else
        createNewClient(client_fd);
}
