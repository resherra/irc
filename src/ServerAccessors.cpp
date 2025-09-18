#include "../headers/Server.hpp"

int Server::getSockFd() {
    return sock_fd.getFd();
}

int Server::getFdCount() {
    return fd_count;
}

struct pollfd* Server::getPfds()
{
    return pfds;
}

void Server::inst_poll()
{
    pfds = new pollfd[fd_size];
    pfds[0].fd = sock_fd.getFd();
    pfds[0].events = POLLIN;
    fd_count = 1;
}