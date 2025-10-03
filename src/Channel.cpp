#include "../headers/Channel.hpp"

bool    Channel::is_empty(){
    return members.empty();
}

void    Channel::setTopic(string to){
    topic = to;
}

string&    Channel::getTopic(){
    return topic;
}

void    Channel::addMember(Client& cli){
    members.push_back(cli);
}

vector<Client>& Channel::getMembers(){
    return members;
}

bool Channel::isMember(string& nick){
    for (size_t i = 0; i < members.size(); i++){
        if(members[i].getNickname() == nick)
            return true;
        }
    return false;
}

bool Channel::isMod(string nick){
    return moderators.count(nick) > 0;
}

void    Channel::addModerator(string cli){
    moderators.insert(cli);
}

set<string> &Channel::getModerators(){
    return moderators;
}

bool Channel::isTopicRestricted(){
    return this->topic_resticted;
}
void Channel::setTopicRestricted(bool value){
    topic_resticted = value;
}

void Channel::setUserLimit(int limit){
    this->user_limit = limit;
}

void Channel::removeUserLimit(void){
    this->user_limit = 0;
}

int Channel::getUserLimit(void){
    return this->user_limit;
}

void Channel::setKey(string key){
    this->key = key;
}

string Channel::getKey(void){
    return this->key;
}

bool Channel::hasKey(void){
    return !key.empty();
}

bool Channel::checkKey(string &key){
     if (key.empty())
        return true;
    return this->key == key;
}

void Channel::removekey(void){
    this->key.clear();
}

void Channel::setinviteonly(bool value){
    this->inv_only = value;
}

bool Channel::isinviteonly(void) const{
    return this->inv_only;
}

void Channel::addinvite(string &nick){
    invitelist.insert(nick);
    cout <<  "the " + nick + " added to the invite list" << endl;

}

bool Channel::isinvited(string &nick){
    return invitelist.count(nick) > 0;
}

void Channel::removeinvite(string &nick){
    invitelist.erase(nick);
     cout <<  "the " + nick + " removed from the invite list" << endl;
}

void Channel::removeMember(string& nick){
    for (vector<Client>::iterator it = members.begin(); it != members.end(); ++it){
        if(it->getNickname() == nick){

            members.erase(it);
            cout << "the member: " + nick << " erased frome the members set" << endl;
            break;
        }
    }

    if(moderators.erase(nick) > 0)
         std::cout << "the member: " + nick << " erased frome the ops set" << endl;
}

void Channel::removeModerator(string nick){
    moderators.erase(nick);
}

string Channel::getModeString(){
    string modes = "+";
    string params = "";
    if(inv_only)
        modes += "i";
    if(topic_resticted)
        modes += "t";

    if(!getKey().empty()){
        modes += "k";
        if(params.empty()){
            params += getKey();
        }
    }

    if(getUserLimit() > 0){
        modes += "l";
        if(!params.empty())
            params += " ";
        ostringstream oss;
        oss << getUserLimit();
        params += oss.str();
    }

    if(modes == "+")
        return "+";
    if (params.empty())
        return modes;
    else
        return modes + " " + params;
}
