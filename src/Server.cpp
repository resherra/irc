#include "../headers/Server.hpp"

void Server::pollcl(){
    if (poll(pfds.data(), fd_count, -1) == -1){   
            close_fds(this->pfds);
            cerr << "poll" << "\n";
            exit(1);
    }
}

void    Server::handle_connections(){
    for (int i = 0; i < fd_count; i++)
    { 
        if (pfds[i].revents & (POLLIN | POLLHUP))
        {
            if (pfds[i].fd == sock_fd.getFd())
                newClient();
            else
                handleClientData(i);
        }
    }
}

Server::Server(string port, string password):   port(atoi(port.c_str())), 
                                                password(password),
                                                sock_fd(port),
                                                fd_count(0), 
                                                fd_size(5) {}


void Server::close_fds(vector<struct pollfd>   &pfds){
    for (size_t i = 0; i <pfds.size(); i++){
            close(pfds[i].fd);
        }

}

bool Server::checkParams(string &line, string &cmd, Client & cl, int sender_fd){
    if(cmd == "PASS"){
        if(line.length() <= 5 || line[4] != ' '){
            string reply = ":myirc 461 " + cl.getNickname() + " PASS :Not enough parameters\r\n";
            send(sender_fd, reply.c_str(), reply.length(), 0);
            return false;
        }

        string pass = line.substr(5);
        if(pass.empty() || pass.find_first_not_of(" \t") == string::npos){
            string reply = ":myirc 461 " + cl.getNickname() + " PASS :Not enough parameters\r\n";
            send(sender_fd, reply.c_str(), reply.length(), 0);
            return false;
        }
    }

    else if(cmd == "NICK"){
        if(line.length() <= 5){
            string reply = ":myirc 461 NICK :Not enough parameters\r\n";
            send(sender_fd, reply.c_str(), reply.length(), 0);
            return false;
        }

        string nick = line.substr(5);
        if(nick.empty() || nick.find_first_not_of(" \t") == string::npos){
            string reply = ":myirc 461 NICK :Not enough parameters\r\n";
            send(sender_fd, reply.c_str(), reply.length(), 0);
            return false;
        }
    }

    else if(cmd == "USER"){
        if(line.length() <= 5 || line[4] != ' '){
            string reply = ":myirc 461 " + cl.getNickname() + " USER :Not enough parameters\r\n";
            send(sender_fd, reply.c_str(), reply.length(), 0);
            return false;
        }
        string user = line.substr(5);
        if(user.empty() || user.find_first_not_of(" \t") == string::npos){
            string reply = ":myirc 461 " + cl.getNickname() + " USER :Not enough parameters\r\n";
            send(sender_fd, reply.c_str(), reply.length(), 0);
            return false;
        }
    }
    return true;
}
