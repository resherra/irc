#include "../headers/Server.hpp"

using   namespace std;

int main(int ac, char **av)
{
    if (ac != 3)
    {
        cerr << "Usage: " << av[0] << " <port> <password>" << "\n";
        exit(1);
    }
    Server serv(av[1], av[2]);
    cout << "server: waiting for connections on port " << serv.getPort() << "\n";
    serv.inst_poll();    
    while (1)
    {
        serv.pollcl();
        serv.handle_connections();
    }
}
