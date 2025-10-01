#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <string>
#include <set>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <sstream>

using namespace std;

class Channel
{
    private:
        string          name;
        string          topic;

        vector<Client>  members;
        set<string>     moderators;
        set<string> invitelist;

        bool inv_only;
        bool topic_resticted;
        int user_limit;
        string key;
        
    public:
        Channel():inv_only(false), topic_resticted(false), user_limit(0) {};
        Channel(string name): name(name), inv_only(false), topic_resticted(false), user_limit(0){};

        bool    is_empty();
        void    addMember(Client& cli);
        vector<Client>& getMembers();
        set<string> &getModerators();
        void    addModerator(string cli);
        void removeModerator(string cli);
        string getModeString();
        string&    getTopic();
        void    setTopic(string to);

        bool isTopicRestricted();
        void setTopicRestricted(bool value);

        //user limit
        void setUserLimit(int limit);
        void removeUserLimit(void);
        int getUserLimit(void);
        
        //key
        void setKey(string key);
        string getKey(void);
        bool hasKey(void);
        bool checkKey(string &key);
        void removekey(void);

        //mode invite
        void setinviteonly(bool value);
        bool isinviteonly(void) const ;
        void addinvite(string &nick);
        bool isinvited(string &nick);
        void removeinvite(string &nick);

        bool isMember(string &);
        bool isMod(string );
        void removeMember(string &);
        
};

#endif