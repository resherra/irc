#include "../headers/Server.hpp"

void Server::handle_client_data(int *pfd_i)
{
    char buffer[256];

    int nbytes = recv(pfds[*pfd_i].fd, buffer, sizeof buffer, 0);

    int sender_fd = pfds[*pfd_i].fd;
    
    if (nbytes <= 0)
    {
        if (nbytes == 0)
            cerr << "socket " << sender_fd << " hung up" << endl;
        else
            cerr << "recv" << endl;
        clients.erase(sender_fd);
        close(sender_fd);
    }
    else
    {
        Client client = clients[sender_fd];
        string msg(buffer, nbytes);

        string::size_type pos;
        while ((pos = msg.find("\r\n")) != std::string::npos) 
        {
            string line = msg.substr(0, pos);
            msg.erase(0, pos + 2);

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
            }
            else if (line.find("CAP LS") == 0) {
                string reply = "CAP * LS :\r\n";
                send(sender_fd, reply.c_str(), reply.length(), 0);
            }
            else if (line.find("NICK ") == 0)
            {
                string nickname = line.substr(5);
                client.setNickname(nickname);
            } else if (line.find("USER ") == 0)
            {
                string username = line.substr(5, line.find(" ", 5) - 5);
                client.setUsername(username);
                string nickname = client.getNickname();
                if (nickname.length() != 0)
                {
                    stringstream reply;
                    reply  << ":myirc 001 " << nickname << " :Welcome to MyIRC\r\n";
                    send(sender_fd, reply.str().c_str(), reply.str().length(), 0);
                }
            }
            else if (line.find("PING ") == 0)
            {
                std::string reply = "PONG : active\r\n";
                send(pfds[*pfd_i].fd, reply.c_str(), reply.length(), 0);
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