#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "Client.hpp"
#include "Channel.hpp"

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
        vector<struct pollfd> pfds;

        map<int, Client>         clients;
        map<string, Channel>     channels;

        void createNewClient(int newfd);
        void newClient();

        void handle_client_data(int);

    public:
        Server(string port, string password);
        Server();
        ~Server();

        int             getSockFd();
        int             getFdCount();

        void            inst_poll();
        void            handle_connections();

        void            pollcl();



        //handlers
        void    cap(int);
        void    nick(Client&, string);
        void    user(Client&, string, int);
        void    ping(int);
};

#endif