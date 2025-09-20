#include "../headers/Server.hpp"

void Server::handle_client_data(int index)
{
    char buffer[256];

    int nbytes = recv(pfds[index].fd, buffer, sizeof buffer, 0);    
    int sender_fd = pfds[index].fd;
    
    if (nbytes <= 0)
    {
        if (nbytes == 0)
            cerr << "socket " << sender_fd << " hung up" << endl;
        else
            cerr << "recv" << endl;
        clients.erase(sender_fd);
        close(sender_fd);    
        fd_count--;
        //to-do: remove it from pfds
        return;
    }
    else
    {
        Client& client = clients[sender_fd];

        string msg(buffer, nbytes);
        client.setMessage(msg);
        string::size_type pos;
        while ((pos = client.getMessage().find("\r\n")) != std::string::npos)
        {
            string line = client.getMessage().substr(0, pos);
            client.getMessage().erase(0, pos + 2);

            std::cout << "<<<" << line << ">>>" << std::endl;

            if (line.find("PASS ") == 0)
            {
                string pass = line.substr(5);
                if (pass != password)
                {
                    std::string reply = ":myirc 464 * :Password incorrect\r\n";
                    send(sender_fd, reply.c_str(), reply.length(), 0);
                    clients.erase(sender_fd);
                    close(sender_fd);
                }
                client.setAuth();
            }
            if (client.getAuth())
            {
                if (line.find("CAP LS") == 0)
                    Server::cap(sender_fd);
                else if (line.find("NICK ") == 0)
                    Server::nick(client, line);
                else if (line.find("USER ") == 0)
                    Server::user(client, line, sender_fd);
                else if (line.find("PING ") == 0)
                    Server::ping(sender_fd);
                else if (line.find("JOIN ") == 0)
                {
                    cout << "enters" << endl;
                    string channel = line.substr(5);
                    string reply;
                    if (channel[0] != '#')
                    {
                        reply = ":myirc 403 " + client.getNickname() + " " + channel + " :No such channel\r\n";
                        send(sender_fd, reply.c_str(), reply.length(), 0);
                        return;
                    }
                    channels[channel] = Channel(channel);
                    channels[channel].addMember(client);

                    reply = ":" + client.getNickname() + "!user@host JOIN :" + channel + "\r\n" 
                            + ":myirc 353 " + client.getNickname() + " = " + channel + " :" + client.getNickname() + "\r\n" 
                            + ":myirc 366 " + client.getNickname() + " " + channel + " :End of /NAMES list\r\n";
                    send(sender_fd, reply.c_str(), reply.length(), 0);
                }
            }

        }

        // Client client = clients[sender_fd];
        // Broadcast
        // for (int j = 0; j < fd_count; j++)
            // {
            //     int dest_fd = pfds[j].fd;
            //     // Except the listener and ourselves
            //     if (dest_fd != sock_fd.getFd() && dest_fd != sender_fd)
            //     {
            //         if (send(dest_fd, buffer, nbytes, 0) == -1)
            //         {
            //             perror("send");
            //         }
            //     }
            // }
        }
}