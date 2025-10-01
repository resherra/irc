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

int main(int ac, char **av)
{
    if (ac != 3)
    {
        cerr << "Usage: " << av[0] << " <port> <password>" << "\n";
        exit(1);
    }
    Server serv(av[1], av[2]);
    // signal(SIGINT, handle_signals);

    cout << "server: waiting for connections on port " << serv.getPort() << "\n";
    serv.inst_poll();    
    while (1)
    {
        serv.pollcl();
        serv.handle_connections();
    }
}
