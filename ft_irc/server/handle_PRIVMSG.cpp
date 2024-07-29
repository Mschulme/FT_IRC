#include "IRC_Server.hpp"

void IRC_Server::handle_PRIVMSG(int fd, Message message)
{
	(void) fd;
    std::cout << "Handling PRIVMSG command with parameters: " << message.parameters[0] << std::endl;
}
