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
        pfds.erase(pfds.begin() + index);
        fd_count--;
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

            if (line.find("PASS") == 0)
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
                else if (line.find("NICK") == 0)
                    Server::nick(client, line);
                else if (line.find("USER") == 0)
                    Server::user(client, line, sender_fd);
                else if (line.find("PING") == 0)
                    Server::ping(sender_fd);
                else if (line.find("JOIN") == 0)
                    Server::join(line, client, sender_fd);
                else if (line.find("PRIVMSG") == 0)
                {
                    string target = line.substr(8, line.find(" ", 8) - 8);
                    string msg = line.substr(line.find(':') + 1);
                    string reply = ":" + client.getNickname() + "!user@host PRIVMSG " + target + " :" + msg + "\r\n";
                    if (target[0] == '#')
                    {
                        Server::privmsg_channel(sender_fd, target, reply);
                    } else
                        Server::privmsg_user(sender_fd, target, reply);
                }
            }
        }
    }
}