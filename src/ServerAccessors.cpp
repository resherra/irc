#include "../headers/Server.hpp"

int Server::getSockFd() {
    return sock_fd.getFd();
}

int Server::getFdCount() {
    return fd_count;
}

void Server::inst_poll()
{
    struct pollfd inst;
    inst.fd = sock_fd.getFd();
    inst.events = POLLIN;
    pfds.push_back(inst);
    fd_count = 1;
}

int Server::getPort()
{
    return port;
}
