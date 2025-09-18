#include "../headers/Server.hpp"

void    Server::handle_connections()
{
    for (int i = 0; i < fd_count; i++)
    {
        // Check if someone's ready to read
        if (pfds[i].revents & (POLLIN | POLLHUP))
        {
            // We got one!!
            if (pfds[i].fd == sock_fd.getFd())
            {
                // If we're the listener, it's a new connection
                newClient();
            }
            else
            {
                // Otherwise we're just a regular client
                // handle_client_data(listener, fd_count, *pfds, &i);
                handle_client_data(&i);
            }
        }
    }
}


Server::Server(string port, string password) : port(atoi(port.c_str())), password(password), fd_count(0), fd_size(5) {}

Server::Server()
{
}

Server::~Server()
{
    delete[] pfds;
}