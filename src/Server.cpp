#include "../headers/Server.hpp"

void Server::pollcl()
{
    if (poll(pfds.data(), fd_count, -1) == -1)
    {   
            close_fds(this->pfds);
            cerr << "poll" << "\n";
            exit(1);
    }
}

void    Server::handle_connections()
{
    int so_fd  = sock_fd.getFd();   
    
    for (int i = 0; i < fd_count; i++)
    { 
        if (pfds[i].revents & (POLLIN | POLLHUP))
        {
            if (pfds[i].fd == so_fd)
                newClient();
            else
                handleClientData(i);
        }
    }
}

Server::Server(string port, string password):   port(atoi(port.c_str())), 
                                                password(password),
                                                sock_fd(port),
                                                fd_count(0) {}


void Server::close_fds(vector<struct pollfd>   &pfds)
{
    for (size_t i = 0; i <pfds.size(); i++)
    {
            close(pfds[i].fd);
    }

}
