#include "IRC_Server.hpp"

void IRC_Server::handle_USER(int fd, std::vector<std::string> message)
{
    //client_list[fd].Username = message.parameters[0];
    std::cout << "For the FD: " << fd << " - the Username is" << message[0] << std::endl;
}
