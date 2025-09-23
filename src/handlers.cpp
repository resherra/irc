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

void    Server::privmsg_channel(int sender_fd, string target, string reply)
{
    if (channels.find(target) != channels.end())
    {
        vector<Client> &cls = channels[target].getMembers();
        for (vector<Client>::iterator it = cls.begin(); it != cls.end(); ++it)
        {
            if ((*it).getFd() != sender_fd)
            {
                send((*it).getFd(), reply.c_str(), reply.length(), 0);
            }
        }
    }
}

void Server::privmsg_user(int sender_fd, string target, string reply)
{
    bool user_exist = false;
    for (map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        Client cl = (*it).second;
        int fd = (*it).first;
        if (cl.getNickname() == target)
        {
            send(fd, reply.c_str(), reply.length(), 0);
            user_exist = true;
            break;
        }
    }
    if (!user_exist)
    {
        std::string err = ":myirc 401 " + target + " :no such nick\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
    }
}

void    Server::join(string line, Client client, int sender_fd)
{
    string channel = line.substr(5);
    string reply;
    if (channel[0] != '#')
    {
        reply = ":myirc 403 " + client.getNickname() + " " + channel + " :no such channel\r\n";
        send(sender_fd, reply.c_str(), reply.length(), 0);
        return;
    }
    if (channels.find(channel) == channels.end())
    {
        channels[channel] = Channel(channel);
    }
    channels[channel].addMember(client);
    
    reply = ":" + client.getNickname() + "!user@host JOIN :" + channel + "\r\n" 
            + ":myirc 353 " + client.getNickname() + " = " + channel + " :" + client.getNickname() + "\r\n" 
            + ":myirc 366 " + client.getNickname() + " " + channel + " :End of /NAMES list\r\n";
    send(sender_fd, reply.c_str(), reply.length(), 0);
}