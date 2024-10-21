#include "IRC_Server.hpp"

void IRC_Server::handle_JOIN(int fd, Message message)
{
	(void) fd;
    std::cout << "Handling JOIN command with parameters: " << message.parameters[0] << std::endl;
}
