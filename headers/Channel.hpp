#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"

#include <string>
#include <set>
#include <vector>


using namespace std;

class Channel
{
    private:
        string          name;
        string          topic;

        vector<Client>  members;
        set<string>     moderators;
        
    public:
        Channel() {};
        Channel(string name): name(name) {};

        bool    is_empty()
        {
            return members.empty();
        }

        void    addMember(Client& cli)
        {
            members.push_back(cli);
        }

        vector<Client>& getMembers()
        {
            return members;
        }

        set<string> &getModerators()
        {
            return moderators;
        }

        void    addModerator(string cli)
        {
            moderators.insert(cli);
        }

        string&    getTopic()
        {
            return topic;
        }

        void    setTopic(string to)
        {
            topic = to;
        }
};

#endif