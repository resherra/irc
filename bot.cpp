#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <ctime>

int main() {

     std::cout << " _______     ______  ___________  \n"
                 "|   _  \"\\   /    \" \\(\"     _   \") \n"
                 "(. |_)  :) // ____  \\)__/  \\\\__/  \n"
                 "|:     \\/ /  /    ) :)  \\\\_ /     \n"
                 "(|  _  \\\\(: (____/ //   |.  |     \n"
                 "|: |_)  :)\\        /    \\:  |     \n"
                 "(_______/  \\\"_____/      \\__|wkdaa    \n" <<std::endl;
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6667);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return 1;
    }

    std::string pass = "PASS 123\r\n";
    std::string nick = "NICK bot\r\n";
    std::string user = "USER bot 0 * :My IRC bot\r\n";

    send(sock, pass.c_str(), pass.size(), 0);
    send(sock, nick.c_str(), nick.size(), 0);
    send(sock, user.c_str(), user.size(), 0);

    char buf[512];
    while (1) {
        int n = recv(sock, buf, sizeof(buf) - 1, 0);
        if (n <= 0) break;
        buf[n] = '\0';
        std::string msg(buf);
        std::cout << msg;
        std::cout << "[" <<  msg << "]" << std::endl;
        if (msg.find(" 001 ") != std::string::npos) {
            // std::string join = "JOIN #irc_bot\r\n";
            // send(sock, join.c_str(), join.size(), 0);
            // send(sock, join.c_str(), join.size(), 0);
        }
        std::cout << msg << std::endl;
        if (msg.find("PRIVMSG bot :!time") != std::string::npos) {
            time_t now = time(NULL);
            std::string reply = "PRIVMSG "+ msg.substr(msg.find(":")+1,msg.find("!")-1) +" :" + std::string(ctime(&now)) + "\r\n";
            send(sock, reply.c_str(), reply.size(), 0);
        }
    }

    close(sock);
    return 0;
}
