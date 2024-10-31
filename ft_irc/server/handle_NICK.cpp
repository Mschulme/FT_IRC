#include "IRC_Server.hpp"

void IRC_Server::handle_NICK(int fd, std::vector<std::string> message)
{
    //client_list[fd].Nickname = message[0];
    std::cout << "For the FD: " << fd << " - the Nickname is" << message[0] << std::endl;
}
