#include "../headers/Server.hpp"

void    Server::cap(int sender_fd)
{
    string reply = "CAP * LS :\r\n";
    send(sender_fd, reply.c_str(), reply.length(), 0);
}


void    Server::nick(Client& client, string line)
{
    string nickname = line.substr(5);
    client.setNickname(nickname);
}

void    Server::user(Client& client, string line, int sender_fd)
{
    string username = line.substr(5, line.find(" ", 5) - 5);
    client.setUsername(username);
    string nickname = client.getNickname();
    if (nickname.length() != 0)
    {
        stringstream reply;
        reply << ":myirc 001 " << nickname << " :Welcome to MyIRC\r\n";
        send(sender_fd, reply.str().c_str(), reply.str().length(), 0);
    }
}

void    Server::ping(int sender_fd)
{
    std::string reply = "PONG : active\r\n";
    send(sender_fd, reply.c_str(), reply.length(), 0);
}
