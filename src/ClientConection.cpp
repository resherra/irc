#include "../headers/Server.hpp"

void Server::createNewClient(int newfd)
{
    std::pair<int, Client> newClient(newfd, Client());
    clients.insert(newClient);

    struct pollfd new_fd;
    new_fd.fd = newfd;
    new_fd.events = POLLIN;
    new_fd.revents = 0;
    pfds.push_back(new_fd);

    fd_count++;
}

void Server::newClient()
{
    struct sockaddr_storage remoteaddr;
    socklen_t               addrlen;
    int                     client_fd;
    // char                    remoteIP[INET6_ADDRSTRLEN];

    addrlen = sizeof remoteaddr;
    client_fd = accept(sock_fd.getFd(), (struct sockaddr *)&remoteaddr,
                       &addrlen);
    if (client_fd == -1)
        perror("accept");
    else
    {
        // struct sockaddr_in *ipv4 = (struct sockaddr_in *)&remoteaddr;
        createNewClient(client_fd);
        // std::cout << "new connection from " << inet_ntoa(ipv4->sin_addr) << " on socket " << client_fd << '\n';
    }
}