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
        int                     port;
        string                  password;
        
        Socket                  sock_fd;
        int                     fd_count;
        int                     fd_size;
        vector<struct pollfd>   pfds;

        map<int, Client>        clients;
        map<string, Channel>    channels;

        void                    newClient();
        void                    createNewClient(int);
        void                    handle_client_data(int);
        
    public:
        Server(string, string);
        ~Server();

        int                     getSockFd();
        int                     getFdCount();
        int                     getPort();

        void                    inst_poll();
        void                    pollcl();

        void                    handle_connections();

        //handlers      
        void                    cap(int);
        void                    nick(Client&, string);
        void                    user(Client&, string, int);
        void                    ping(int);
        void                    join(string, Client, int);
        void                    privmsg_channel(int, string, string);
        void                    privmsg_user(int, string, string);
};

#endif