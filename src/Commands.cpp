#include "../headers/Server.hpp"

int Server::findCliendFD(const string &nick){

    for (map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it){
        if(it->second.getNickname() == nick)
            return it->first;
    }

    return -1;
}

void Server::kick(string &line, Client &client, int sender_fd){

    istringstream iss(line);
    string cmd, chName, target, reason, kick_msg;
    iss >> cmd >> chName >> target; 
    
    string remaining;
    getline(iss, remaining);
    if (!remaining.empty()) {
        size_t start = remaining.find_first_not_of(" ");
        if (start != string::npos) {
            if (remaining[start] == ':') {
                start++;
            }
            reason = remaining.substr(start);
        }
    }    
    if(channels.find(chName) == channels.end()){
        string err = ":myirc 403 " + client.getNickname() + " " + chName + " :No such channel\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
        return ;
    }

    Channel& channel = channels[chName];
    string clientNick = client.getNickname();
    if(!channel.isMember(clientNick)){
        string err = ":myirc 442 " + client.getNickname() + " " + chName + " :You're not on that channel\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
        return;
    }

    if(!channel.isMod(clientNick)){
        string err = ":myirc 482 " + client.getNickname() + " " + chName + " :You're not channel moderator\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
        return;
    }

    if(!channel.isMember(target)){
        string err = ":myirc 441 " + client.getNickname() + " " + chName + " :They aren't on that channel\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
        return;
    }

    kick_msg = ":" + client.getNickname() + "!" + client.getUsername() + "@host KICK " + chName + " " + target;
    if(!reason.empty())
        kick_msg += ":" + reason;
    kick_msg += "\r\n";
    cout << reason << endl;
    
    
    vector<Client> &members = channel.getMembers();
    for (size_t i = 0; i < members.size(); i++){
        int member_fd = findCliendFD(members[i].getNickname());
        if(member_fd != -1)
            send(member_fd, kick_msg.c_str(), kick_msg.length(), 0);
    }
    
    channel.removeMember(target);
    int target_fd = findCliendFD(target);
    cout << target_fd << endl;
    if(target_fd != -1)
        clients[target_fd].rmfromChannels(chName);
}

void Server::invite(string &line, Client &client, int sender_fd){

    istringstream iss(line);
    string cmd, target, chName;
    iss >> cmd >> target >> chName;


    if(channels.find(chName) == channels.end()){
        string err = ":myirc 403 " + clients[sender_fd].getNickname() + " " + target +  " :No such channel\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
        return ;
    }


    Channel& channel = channels[chName];
    string clientNick = client.getNickname();
    if(!channel.isMember(clientNick)){
        string err = ":myirc 442 " + client.getNickname() + " " + chName + " :You're not on that channel\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
        return;
    }

    int cl_fd = findCliendFD(target);
    if(cl_fd == -1){
        string err = ":myirc 401 " + target + " :No such nick\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
        return ;
    }

    if(channel.isMember(target)){
        string err = "myirc 443" + target + " " + chName + ":is already on channel\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
        return ;
    }

    string invite_msg = ":" + client.getNickname() + "!" + client.getUsername() + "@host INVITE " + target + "\r\n";
    send(cl_fd, invite_msg.c_str(), invite_msg.length(), 0);

}

// void Server::topic(string line, Client client, int sender_fd){

// }

// void Server::mode(string line, Client client, int sender_fd){

// }