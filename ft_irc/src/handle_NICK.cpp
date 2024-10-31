#include "IRC_Server.hpp"

void IRC_Server::handle_NICK(int fd, std::vector<std::string> message)
{
    client_list[fd].set_nickname(message[1]);
    std::cout << "For the FD: " << fd << " - the Nickname is " << message[1] << std::endl;
}
