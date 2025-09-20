#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

using namespace std; 

class Client
{
    private:
        string  username;
        string  nickname;

        string  message;

        bool    auth;
    public: 
        Client(): auth(false) {}
        ~Client() {}

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

        bool    getAuth()
        {
            return auth;
        }
};

#endif