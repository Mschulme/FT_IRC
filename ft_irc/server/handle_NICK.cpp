#include "IRC_Server.hpp"

void IRC_Server::handle_NICK(int fd, Message message)
{
    client_list[fd].Nickname = message.parameters[0];
    std::cout << "For the FD: " << fd << " - the Nickname is" << message.parameters[0] << std::endl;
}
