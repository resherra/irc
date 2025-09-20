#include "../headers/Server.hpp"

#include <netdb.h>
#include <arpa/inet.h>

using   namespace std;

struct sockaddr_storage their_addr; // connector's address info
char s[INET6_ADDRSTRLEN];

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void    Server::pollcl()
{
    if (poll(pfds.data(), fd_count, -1) == -1)
    {
        cerr << "poll" << "\n";
        std::exit(1);
    }
}

int main(int ac, char **av)
{
    (void)ac;
    (void) av;
    if (ac != 3)
    {
        cerr << "Usage: " << av[0] << " <port> <password>" << "\n";
        exit(1);
    }
    Server serv(av[1], av[2]);
    
    serv.inst_poll();    
    cout << "server: waiting for connect ions on port " << PORT << "\n";
    while (1)
    {
        serv.pollcl();
        serv.handle_connections();
    }
}
