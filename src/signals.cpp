

#include "../headers/Server.hpp"






void handle_signals(int sig, Server &serv)
{
    (void)sig;
    for (size_t i = 0; i < serv.getPollfds().size(); i++)
    {
        close( serv.getPollfds()[i].fd);
    }
    // free and close();
}