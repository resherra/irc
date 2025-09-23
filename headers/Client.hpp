#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

using namespace std; 

class Client
{
    private:
        int     fd;
        string  username;
        string  nickname;

        string  message;
        bool    auth;
    public: 
        Client(): auth(false) {}
        Client(int fd) : fd(fd), auth(false) {}
        ~Client() {}

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
            cout << "enters" << endl;
            message += msg;
        }

        void    setAuth()
        {
            auth = true;
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
};

#endif