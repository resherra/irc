#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"

#include <string>
#include <map>
#include <vector>

using namespace std;

class Channel
{
    private:
        string          name;
        vector<Client>  members;

    public:
        void    addMember(Client cli)
        {
            members.push_back(cli);
        }
        Channel() {};
        Channel(string name): name(name) {};
};

#endif