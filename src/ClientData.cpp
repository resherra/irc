#include "../headers/Server.hpp"

void Server::handleClientData(int index)
{
    char buffer[512];

    int sender_fd = pfds[index].fd;
    int nbytes = recv(sender_fd, buffer, sizeof buffer, 0);    
    
    if (nbytes <= 0)
    {
        if (nbytes)
            cerr << "recv" << endl;
        Server::quit(clients[sender_fd], "nc", sender_fd, index);
    }
    else
    {
        Client& client = clients[sender_fd];
        string msg(buffer, nbytes);
        client.setMessage(msg);

        string::size_type pos = client.getMessage().find("\r\n");
        while (pos != std::string::npos)
        {
            string line = client.getMessage().substr(0, pos);
            client.getMessage().erase(0, pos + 2);

            string::size_type space_pos = line.find(" ");
            string  cmd = line.substr(0, space_pos);


            if (cmd == "PASS" && !client.getAuth())
            {
                if(Server::checkParams(line, cmd, client, sender_fd) == false)
                    break;
                string pass = line.substr(5);

                if (pass != password){
                    string reply = ":myirc 464 * :Password incorrect\r\n";
                    send(sender_fd, reply.c_str(), reply.length(), 0);
                } else
                {
                    client.setAuth();
                }
                break;
            }
            if (client.getAuth())
            {
                if (line.find("CAP LS ") == 0)
                    Server::cap(sender_fd);
                else if (cmd == "NICK"){
                    if(Server::checkParams(line, cmd, client, sender_fd) == false)
                        break;
                    Server::nick(client, line, sender_fd);
                }
                else if (cmd == "USER"){
                    if(Server::checkParams(line, cmd, client, sender_fd) == false)
                        break ;
                    Server::user(client, line, sender_fd);
                }
                else if (client.getRegistred())
                {
                    if (cmd == "PING")
                        Server::ping(sender_fd);
                    else if (cmd == "JOIN")
                        Server::join(line, client, sender_fd);
                    else if (cmd == "PRIVMSG")
                    {
                        string::size_type space_pos = line.find(" ", 8);
                        if (space_pos != string::npos && space_pos + 2 < line.length() && line[space_pos + 1] == ':')
                        {
                            string target = line.substr(8, space_pos - 8);
                            string msg = line.substr(space_pos + 2);
                            string reply = ":" + client.getNickname() + "!" + client.getUsername() + "@host PRIVMSG " + target + " :" + msg + "\r\n";
                            cout << msg <<endl;
                            if (target[0] == '#')
                                Server::privmsg_channel(sender_fd, target, reply, false);
                            else
                                Server::privmsg_user(sender_fd, target, reply, msg, client);
                        }
                        else
                        {
                            string err = ":myirc 461 " + client.getNickname() + " PRIVMSG :Not enough parameters\r\n";
                            send(sender_fd, err.c_str(), err.length(), 0);
                        }
                    }
                    else if (cmd == "PART")
                        Server::part(client, line, sender_fd);
                    else if(cmd == "KICK")
                        Server::kick(line, client, sender_fd);
                    else if(cmd == "INVITE")
                        Server::invite(line, client, sender_fd);
                    else if(cmd == "TOPIC")
                        Server::topic(line, client, sender_fd);
                    else if(cmd == "MODE")
                        Server::mode(line, client, sender_fd);
                    else if (cmd == "QUIT"){
                        Server::quit(client, line, sender_fd, index);
                        return;   
                    }
                    else{
                        stringstream reply;
                        reply << ":myirc 421 " << client.getNickname() << " " << cmd << " :Unknown command\r\n";
                        send(sender_fd, reply.str().c_str(), reply.str().length(), 0);
                    }
                } else{
                    stringstream reply;
                    reply << ":myirc 451 * " << ":You have not registered\r\n";
                    send(sender_fd, reply.str().c_str(), reply.str().length(), 0);
                }
            }
            pos = client.getMessage().find("\r\n");
        }
    }
}
