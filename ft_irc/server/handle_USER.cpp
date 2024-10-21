#include "IRC_Server.hpp"

void IRC_Server::handle_USER(int fd, Message message)
{
    client_list[fd].Username = message.parameters[0];
    std::cout << "For the FD: " << fd << " - the Username is" << message.parameters[0] << std::endl;
}
