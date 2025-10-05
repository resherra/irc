#include "../headers/Server.hpp"

void Server::pollcl()
{
    if (poll(pfds.data(), fd_count, -1) == -1)
    {
        // -1 also returned if a signal arrived
        
            close_fds(this->pfds);
            cerr << "poll" << "\n";
            std::exit(1);// 
       
        
        // std::exit(1);
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
                handleClientData(i);
        }
    }
}

Server::Server(string port, string password):   port(atoi(port.c_str())), 
                                                password(password),
                                                sock_fd(port),
                                                fd_count(0), 
                                                fd_size(5) {}

// Server::~Server()
// {
// }


void Server::close_fds(vector<struct pollfd>   &pfds)
        {
            for (size_t i = 0; i <pfds.size(); i++)
                {
                    std::cout << "df" << std::endl;
                    close(pfds[i].fd);
                }

        }





// vector<struct pollfd> Server::getPollfds()
// {
//     return (this->pfds);

            
//         }