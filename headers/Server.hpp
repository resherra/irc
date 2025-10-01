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
#include <cstdlib>
#include <cstdio>




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
        void                    handleClientData(int);
        
    public:
        Server(string, string);
        ~Server();

        int                     getSockFd();
        int                     getFdCount();
        int                     getPort();

        void                    inst_poll();
        void                    pollcl();

        void                    handle_connections();

        // get all FDs 
        vector<struct pollfd> getPollfds();
        
        //handlers      
        void                    cap(int);
        void                    nick(Client&, string, int);
        void                    user(Client&, string, int);
        void                    register_user(Client&, int);
        void                    ping(int);
        void                    join(string, Client&, int);
        void                    privmsg_channel(int, string, string, bool join_bool);
        void                    privmsg_user(int, string, string);
        void                    part(Client&, string, int);
        void                    quit(Client&, string, int, int);
        void                    exit(Client&, string, int, int);
   

        //commands
        void kick (string&, Client&, int);
        void invite (string&, Client&, int);
        void topic (string, Client, int);
        void mode (string, Client&, int);

        //helpers func
        int findCliendFD(const string &nick);
};


void handle_signals(int sig, Server &serv);
#endif