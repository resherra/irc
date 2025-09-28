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

bool Channel::isMod(string &nick){
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
    return key.empty() || this->key == key;
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
