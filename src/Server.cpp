#include "../headers/Server.hpp"

void Server::pollcl()
{
    if (poll(pfds.data(), fd_count, -1) == -1)
    {
        cerr << "poll" << "\n";
        std::exit(1);
    }
}

void    Server::handle_connections()
{
    for (int i = 0; i < fd_count; i++)
    { 
        if (pfds[i].revents & (POLLIN | POLLHUP))
        {
            if (pfds[i].fd == sock_fd.getFd())
                newClient();
            else
                handle_client_data(i);
        }
    }
}

Server::Server(string port, string password):   port(atoi(port.c_str())), 
                                                password(password),
                                                sock_fd(port),
                                                fd_count(0), 
                                                fd_size(5) {}

Server::~Server()
{
}