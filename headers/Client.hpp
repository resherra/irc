#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

using namespace std; 

class Client
{
    private:
        string username;
        string nickname;

        string message;
    
    public: 
        Client() {}
        ~Client() {}


        void    setNickname(string nick) {
            nickname = nick;
        }

        void setUsername(string user)
        {
            username = user;
        }

        string getNickname() {
            return nickname;
        }

        string getUsername()
        {
            return username;
        }

        // std::string& getMessage() {
        //     return message;
        // }
};

#endif