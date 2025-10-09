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

    reply << ":myirc 001 " << nickname << " :Welcome to the Internet Relay Network " << nickname << "!" << username << "@host\r\n";
    reply << ":myirc 002 " << nickname << " :Your host is myirc, running version 1.0\r\n";
    reply << ":myirc 003 " << nickname << " :This server was created 09/25\r\n";
    reply << ":myirc 004 " << nickname << " :myirc 1.0 ao mtov\r\n";

    send(sender_fd, reply.str().c_str(), reply.str().length(), 0);
    client.setRegistred();
}

void    Server::nick(Client& client, string line, int sender_fd)
{
    string nickname = line.substr(5);

    if (client.getRegistred())
    {
        stringstream reply;
        reply << ":myirc 462 * " << ":Unauthorized command (already registered)\r\n";
        send(sender_fd, reply.str().c_str(), reply.str().length(), 0);
        return;
    }

    if (nickname.empty())
    {
        stringstream reply;
        reply << ":myirc 431 * " << ":No nickname given\r\n";
        send(sender_fd, reply.str().c_str(), reply.str().length(), 0);
        return;
    }

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
    if (client.getRegistred())
    {
        stringstream reply;
        reply << ":myirc 462 * " << ":Unauthorized command (already registered)\r\n";
        send(sender_fd, reply.str().c_str(), reply.str().length(), 0);
        return;
    }

    string username = line.substr(5, line.find(" ", 5) - 5);
    if (username.empty())
    {
        stringstream reply;
        reply << ":myirc 461 * " << "USER :Not enough parameters\r\n";
        send(sender_fd, reply.str().c_str(), reply.str().length(), 0);
        return;
    }

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
        Channel& channel = channels[target];
        string senderNick = clients[sender_fd].getNickname();
        
        if (!channel.isMember(senderNick)) {
            string err = ":myirc 404 " + senderNick + " " + target + " :Cannot send to channel\r\n";
            send(sender_fd, err.c_str(), err.length(), 0);
            return;
        }

        vector<Client> &cls = channels[target].getMembers();
        for (vector<Client>::iterator it = cls.begin(); it != cls.end(); ++it)
        {
            if (join_bool)
                send((*it).getFd(), reply.c_str(), reply.length(), 0);
            else if ((*it).getFd() != sender_fd)
                send((*it).getFd(), reply.c_str(), reply.length(), 0);
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
    istringstream iss(line);
    string cmd, channelName, key;
    iss >> cmd >> channelName >> key;

    string nickname = client.getNickname();
    string reply;

    if (channelName[0] != '#')
    {
        reply = ":myirc 476 " + nickname + " " + channelName + " :Bad Channel Mask\r\n";
        send(sender_fd, reply.c_str(), reply.length(), 0);
        return;
    }

    // check if channel doesnt already exist
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

    //chedk chnnel invite only and user not invited
    if(channel.isinviteonly() && !channel.isinvited(nickname)){
        reply = ":myirc 473 " + nickname + " " + channelName + " :Cannot join channel (+i)\r\n";
        send(sender_fd, reply.c_str(), reply.length(), 0);
        return;
    }

    //check channel key
    if(channel.hasKey()){
        if(key.empty() || !channel.checkKey(key))
        {

            reply = ":myirc 475 " + nickname + " " + channelName + " :Cannot join channel (+k)\r\n";
            send(sender_fd, reply.c_str(), reply.length(), 0);
            return;
        }
    }

    //check channel user limit
    if(channel.getUserLimit() > 0 && (int)members.size() >= channel.getUserLimit()){
        reply = ":myirc 471 " + nickname + " " + channelName + " :Cannot join channel (+l)\r\n";
        send(sender_fd, reply.c_str(), reply.length(), 0);
        return;
    }

    // int bot_is_up = 0;
    // for (size_t i = 0; i < this->clients.size()       ; i++)
    // {
    //     if (this->clients[i].getNickname() == "bot")
    //         {
    //             send(this->clients[i].getFd(), "start_connection", 17, 0);
    //             bot_is_up = 1;
    //         }

    //     std::cout << this->clients[i].getNickname() <<  "<<<" << std::endl;
    // }
    
    channel.addMember(client);
    client.addtoChannels(channelName);

    if(channel.isinvited(nickname))
        channel.removeinvite(nickname);
    members = channel.getMembers();

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

void    Server::part(Client& client, string line, int sender_fd)
{
    string::size_type space_pos = line.find(" ", 5);
    if (space_pos != string::npos && space_pos + 2 < line.length() && line[space_pos + 1] == ':')
    {
        string target = line.substr(5, space_pos - 5);
        string msg = line.substr(space_pos + 2);
        string nick = client.getNickname();
        string reply;
        bool is_in = false;

        Channel &channel = channels[target];

        // check if user in
        vector<Client>  &members = channel.getMembers();
        set<string>     &moderators = channel.getModerators();
        for (vector<Client>::iterator it = members.begin(); it != members.end(); ++it)
        {
            if (nick == (*it).getNickname())
            {
                reply = ":" + nick + "!" + client.getUsername() + "@host PART " + target + " :" + msg + "\r\n";
                Server::privmsg_channel(sender_fd, target, reply, false);
                if (moderators.find(nick) != moderators.end())
                    moderators.erase(nick);
                members.erase(it);
                client.rmfromChannels(target);
                is_in = true;                        
                if (channel.is_empty())
                    channels.erase(target);
                break;
            }
        }

        if (!is_in)
        {
            string err = ":myirc 442 " + nick + " " + target + " :You're not on that channel\r\n";
            send(sender_fd, err.c_str(), err.length(), 0);
        }
    } else
    {
        string err = ":myirc 461 " + client.getNickname() + " PART :Not enough parameters\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
    }
}

void Server::quit(Client &client, string line, int sender_fd, int index)
{
    string::size_type space_pos = line.find(':');
    string msg;

    if (space_pos != string::npos && space_pos + 1 < line.length())
        msg = line.substr(space_pos + 1);

    string nick = client.getNickname();
    set<string> Clientchannels = client.getChannels();
    for (set<string>::iterator it_chan = Clientchannels.begin(); it_chan != Clientchannels.end(); ++it_chan)
    {
        Channel &channel = channels[*it_chan];
        vector<Client> &members = channel.getMembers();
        set<string> &moderators = channel.getModerators();

        for (vector<Client>::iterator it = members.begin(); it != members.end(); ++it)
        {
            if (nick == (*it).getNickname())
            {
                string reply = ":" + nick + "!" + client.getUsername() + "@host QUIT" + " :" + (!msg.empty() ? msg : "") + "\r\n";
                Server::privmsg_channel(sender_fd, *it_chan, reply, false);
                if (moderators.find(nick) != moderators.end())
                    moderators.erase(nick);
                members.erase(it);
                if (channel.is_empty())
                    channels.erase(*it_chan);
                break;
            }
        }
    }
    clients.erase(sender_fd);
    close(sender_fd);
    pfds.erase(pfds.begin() + index);
    fd_count--;
    cout << "socket " << sender_fd << " hung up" << endl;
}
