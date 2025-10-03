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

void Server::topic(string line, Client client, int sender_fd){

    istringstream iss(line);
    string cmd, chName, newTop, remainTop;
    iss >> cmd >> chName;

    getline(iss, remainTop);
    if(!remainTop.empty()){
        size_t start    = remainTop.find_first_not_of(" ");
        if(start != string::npos){
            if(remainTop[start] == ':')
                start++;
        }
        newTop = remainTop.substr(start);
    }

    if(channels.find(chName) == channels.end()){
        string err = ":myirc 403 " + client.getNickname() + " " + chName + " :No such channel\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
        return;
    }

    Channel& channel = channels[chName];
    string clientNick = client.getNickname();

    if(!channel.isMember(clientNick)){
        string err = ":myirc 442 " + client.getNickname() + " " + chName + " :You're not on that channel\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
        return;
    }

    if(newTop.empty()){
        string currTopic = channel.getTopic();
        if(currTopic.empty()){
            string rpl = ":myirc 331 " + client.getNickname() + " " + chName + " :No topic is set\r\n";
            send(sender_fd, rpl.c_str(), rpl.length(), 0);
        }
        else{
            string rpl = ":myirc 332 " + client.getNickname() + " " + chName + " :" + currTopic + "\r\n";
            send(sender_fd, rpl.c_str(), rpl.length(), 0);
        }
        return ;
    }

    if(channel.isTopicRestricted() && !channel.isMod(clientNick)){
        string err = ":myirc 482 " + client.getNickname() + " " + chName + " :You're not channel operator\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
        return ;
    }

    channel.setTopic(newTop);
    string topmsg = ":" + client.getNickname() + "!" + client.getUsername() + "@host TOPIC " + chName + " :" + newTop + "\r\n"; 
    
    vector<Client> &members = channel.getMembers();
    for (size_t i = 0; i < members.size(); i++){
        int member_fd = findCliendFD(members[i].getNickname());
        if(member_fd != -1)
            send(member_fd, topmsg.c_str(), topmsg.length(), 0);
    }
}

void Server::mode(string line, Client &client, int sender_fd){
    istringstream iss(line);
    string cmd, chName, modes;
    vector<string> params;

    (void)sender_fd;
    
    iss >> cmd >> chName >> modes;

    std::string modeparams;
    while (iss >> modeparams)
        params.push_back(modeparams);


    cout << "\n\n--------------- line -------------" << endl;
    cout << line << endl;

    cout << "--------------- cmd -------------" << endl;
    cout << cmd << endl;

    cout << "--------------- channel name -------------" << endl;
    cout << chName << endl;

    
    
    
    
    cout << "--------------- modes -------------" << endl;
    cout << modes << endl;
    
    cout << "--------------- modeparams -------------" << endl;
    cout << modeparams << endl;
    
    cout << "---------- params vector ---------------------" << endl;

    for (vector<string>::iterator t= params.begin(); t != params.end(); ++t){
        cout << *t << endl;
    }
    cout << "-------------------------------------\n\n" << endl;
    
    if(channels.find(chName) == channels.end()){
        string err = ":myirc 403 " + client.getNickname() + " " + chName + " :No such channel\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
        return;
    }

    Channel& channel = channels[chName];
    cout << "------- mode string-------" << endl;

    cout << channel.getModeString() << endl;
    cout << "--------------------------\n\n" << endl;

    if (modes.empty()) {
        std::string reply = ":myirc 324 " + client.getNickname() + " " + chName + " " + channel.getModeString() + "\r\n";
        send(sender_fd, reply.c_str(), reply.length(), 0);
        return;
    }
    
    if(!channel.isMod(client.getNickname())){
        string err = ":myirc 482 " + client.getNickname() + " " + chName + " :You're not channel moderator\r\n";
        send(sender_fd, err.c_str(), err.length(), 0);
        return;
    }

    bool set = true;
    size_t index = 0;
    string appliedmode = "";
    
    for (size_t i = 0; i < modes.size(); i++){
        
        bool modePower = false;
        char flag = modes[i];
        
        if(flag == '+'){
            set = true;
            continue;
        }
        
        if(flag == '-'){
            set = false; 
            continue;
        }
        

        string currParam = "";
        switch (flag){

        case 'i':
            channel.setinviteonly(set);
            cout << "channel invite only modified " + set << endl;
            modePower = true;
            break;

        case 't':
            channel.setTopicRestricted(set);
            cout << "channel topic restricted modified " + set << endl;
            modePower = true;
            break;


        case 'k':
        if(set){
            if(index < params.size()){
                channel.setKey(params[index]);
                currParam = params[index];
                index++;
                modePower = true;
                cout << "the channel key appliedd" << endl;
            }
        }
        else{
            channel.removekey();
            cout << "the channel key removed"<< endl;
            modePower = true;
        }
        break;

        case 'o':
        if(index < params.size()){
            string clNick = params[index];
            index++;
            if(channel.isMember(clNick)){
                if(set){
                    channel.addModerator(clNick);
                    cout << clNick + " added as moderator to chanel" << endl;
                }
                else{
                    channel.removeModerator(clNick);
                    cout << clNick + " removed from moderator list " << endl;
                }
                currParam = clNick;
                modePower = true;
            }
        }
            
        break;

        case 'l':
            if(set){
                if(index < params.size()){
                    int limit = atoi(params[index].c_str());
                    if(limit > 0){
                        channel.setUserLimit(limit);
                        currParam = params[index];
                        modePower = true;
                        index++;
                        cout << "the channel user limit applied" << endl;
                    }
                }
            }
            else{
                channel.removeUserLimit();
                cout << "the channel user limit removed" << endl;
            }
            break;
        
        default:
            string err = ":myirc 472 " + client.getNickname() + " " + string(1, flag) + " :is unknown mode char to me\r\n";
            send(sender_fd, err.c_str(), err.length(), 0);
            continue ;
        }

        if(modePower){

            if(appliedmode.empty()){
                if(set)
                    appliedmode += "+";
                else
                    appliedmode += "-";
            }
            else{
                char lstchar = '\0'; 
                if(!appliedmode.empty())
                    lstchar = appliedmode.size() - 1;
                if(set && lstchar == '-')
                    appliedmode += "+";
                else if(!set && lstchar == '+')
                    appliedmode += "-";
            }
            appliedmode += flag;
            
            if(!currParam.empty()){
                if(!modeparams.empty()){
                    modeparams += " ";
                }
                modeparams += currParam;
            }
            
        }
    }

        if(!appliedmode.empty()){
            string modemsg = ":" + client.getNickname() + "!" + client.getUsername() + "@host MODE " +chName+" " + appliedmode;

            if(!modeparams.empty())
                modemsg += " " + modeparams;
            modemsg += "\r\n";

            vector<Client> &members = channel.getMembers();
            for (size_t i = 0; i < members.size(); i++)
            {
                int mem_fd = findCliendFD(members[i].getNickname());
                if(mem_fd != -1)
                    send(mem_fd, modemsg.c_str(), modemsg.length(), 0);
            }
                       
        }
    }