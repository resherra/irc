#include "../headers/Server.hpp"

void Server::createNewClient(int newfd)
{
    clients.insert(std::pair<int, Client>(newfd, Client()));

    // If we don't have room, add more space in the pfds array
    if (fd_count == fd_size)
    {
        int tmp = fd_size;
        fd_size *= 2;
        pollfd *new_pfds = new pollfd[fd_size];
        std::memcpy(new_pfds, pfds, tmp * sizeof(pollfd));
        delete[] pfds;
        pfds = new_pfds;
    }

    pfds[fd_count].fd = newfd;
    pfds[fd_count].events = POLLIN; // Check ready-to-read
    pfds[fd_count].revents = 0;

    fd_count++;
}

void Server::newClient()
{
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;
    int client_fd; // Newly accept()ed socket descriptor
    char remoteIP[INET6_ADDRSTRLEN];

    addrlen = sizeof remoteaddr;
    client_fd = accept(sock_fd.getFd(), (struct sockaddr *)&remoteaddr,
                       &addrlen);

    if (client_fd == -1)
    {
        perror("accept");
    }
    else
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)&remoteaddr;
        createNewClient(client_fd);
        // std::cout << "new connection from " << inet_ntoa(ipv4->sin_addr) << " on socket " << client_fd << '\n';
    }
}