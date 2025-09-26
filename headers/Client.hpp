#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <set>

using namespace std; 

class Client
{
    private:
        int     fd;
        string  username;
        string  nickname;

        string  message;
        bool    auth;
        bool    registred;
        
        
        set<string> channels;

    public:     
        Client(): auth(false) {}
        Client(int fd) : fd(fd), auth(false), registred(false) {}
        ~Client() {}

        void    addtoChannels(string ch)
        {
            channels.insert(ch);
        }

        void rmfromChannels(string ch)
        {
            channels.erase(ch);
        }

        set<string>&    getChannels()
        {
            return channels;
        }

        void    setFd(int f)
        {
            fd = f;
        }

        void    setNickname(string nick) {
            nickname = nick;
        }

        void setUsername(string user)
        {
            username = user;
        }

        void setMessage(string msg)
        {
            message += msg;
        }

        void    setAuth()
        {
            auth = true;
        }

        void setRegistred()
        {
            registred = true;
        }

        string getNickname() {
            return nickname;
        }

        string getUsername()
        {
            return username;
        }

        std::string& getMessage() {
            return message;
        }

        int getFd()
        {
            return fd;
        }

        bool    getAuth()
        {
            return auth;
        }

        bool getRegistred()
        {
            return registred;
        }
};

#endif