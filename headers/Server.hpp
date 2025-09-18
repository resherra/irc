#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "Client.hpp"

#include <string>
#include <poll.h>
#include <map>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <vector>

using namespace std;

class Server
{
    private:
        int             port;
        string          password;
        
        Socket          sock_fd;
        int             fd_count;
        int             fd_size;
        struct pollfd   *pfds;

        map<int, Client> clients;
        map<int, Client> clients;



        void createNewClient(int newfd);
        void newClient();

        void handle_client_data(int *pfd_i);

    public:
        Server(string port, string password);
        Server();
        ~Server();

        int             getSockFd();
        int             getFdCount();
        struct pollfd*  getPfds();

        void            inst_poll();
        void            handle_connections();
};

#endif