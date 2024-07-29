#include "IRC_Server.hpp"

void IRC_Server::handle_USER(int fd, Message message)
{
	(void) fd;
    std::cout << "Handling NICK command with parameters: " << message.parameters[0] << std::endl;
}
