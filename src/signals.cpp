#include "../headers/Server.hpp"


void handle_signals(int sig)
{
    (void)sig;
    signaled = 7;
    // free and close();
}