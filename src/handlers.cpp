#include "../headers/Server.hpp"

void    Server::cap(int sender_fd)
{
    string reply = "CAP * LS :\r\n";
    send(sender_fd, reply.c_str(), reply.length(), 0);
}

void    Server::register_user(Client &client, int sender_fd)
{
    string nickname = client.getNickname();
    string username = client.getUsername();

    stringstream reply;
    reply << ":myirc 001 " << nickname << " :Welcome to the Internet Relay Network "
          << nickname << "!" << username << "@host\r\n";

    send(sender_fd, reply.str().c_str(), reply.str().length(), 0);
    client.setRegistred();
}

void    Server::nick(Client& client, string line, int sender_fd)
{
    string nickname = line.substr(5);

    bool user_exist = false;
    for (map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        Client cl = (*it).second;
        if (cl.getNickname() == nickname)
        {
            stringstream reply;
            reply << ":myirc 433 * " + nickname + " :Nickname is already in use\r\n";
            send(sender_fd, reply.str().c_str(), reply.str().length(), 0);
            user_exist = true;
            break;
        }
    }

    if (!user_exist)
    {
        client.setNickname(nickname);

        string username = client.getUsername();
        if (!username.empty() && !client.getRegistred())
            register_user(client, sender_fd);
    }
}

void    Server::user(Client& client, string line, int sender_fd)
{
    string username = line.substr(5, line.find(" ", 5) - 5);
    client.setUsername(username);

    string nickname = client.getNickname();
    if (!nickname.empty() && !client.getRegistred())
        register_user(client, sender_fd);
}

void    Server::ping(int sender_fd)
{
    std::string reply = "PONG :active\r\n";
    send(sender_fd, reply.c_str(), reply.length(), 0);
}

void    Server::privmsg_channel(int sender_fd, string target, string reply, bool join_bool)
{
    if (channels.find(target) != channels.end())
    {
        vector<Client> &cls = channels[target].getMembers();
        for (vector<Client>::iterator it = cls.begin(); it != cls.end(); ++it)
        {
            if (!join_bool)
                continue;
            if ((*it).getFd() != sender_fd)
            {
                send((*it).getFd(), reply.c_str(), reply.length(), 0);
            }
        }
    }
    else
    {
        string err = ":myirc 403 " + clients[sender_fd].getNickname() + " " + target +  " :No such channel\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
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
        string err = ":myirc 401 " + target + " :No such nick\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
    }
}

void    Server::join(string line, Client& client, int sender_fd)
{
    string channelName = line.substr(5);
    string nickname = client.getNickname();
    string reply;

    if (channelName[0] != '#')
    {
        reply = ":myirc 476 " + nickname + " " + channelName + " :Bad Channel Mask\r\n";
        send(sender_fd, reply.c_str(), reply.length(), 0);
        return;
    }
    
    // check if channel already exist
    if (channels.find(channelName) == channels.end())
    {
        channels[channelName] = Channel(channelName);
        channels[channelName].addModerator(client.getNickname());
    }
    Channel &channel = channels[channelName];

    // check if user already in
    vector<Client> &members = channel.getMembers();
    for (vector<Client>::iterator it = members.begin(); it != members.end(); ++it)
    {
        if (nickname == (*it).getNickname())
            return;
    }

    channel.addMember(client);

    // list of channels members
    string names;
    set<string>& moderators = channel.getModerators();
    for (vector<Client>::iterator it = members.begin(); it != members.end(); ++it)
    {
        string nick = (*it).getNickname();
        if (moderators.find(nick) != moderators.end())
            names += "@";
        names += nick + " ";               
    }


    // forward to members
    reply = ":" + nickname + "!" + client.getUsername() + "@host JOIN :" + channelName + "\r\n";
    Server::privmsg_channel(sender_fd, channelName, reply, true);
    
    // server reply
    string topic_reply = (!channel.getTopic().empty()
                              ? (":myirc 332 " + nickname + " " + channelName + " :" + channel.getTopic() + "\r\n")
                              : (":myirc 331 " + nickname + " " + channelName + " :No topic is set\r\n"));
    reply = topic_reply
            + ":myirc 353 " + nickname + " = " + channelName + " :" + names + "\r\n" 
            + ":myirc 366 " + nickname + " " + channelName + " :End of NAMES list\r\n";
    send(sender_fd, reply.c_str(), reply.length(), 0);
}