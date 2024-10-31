#include "IRC_Server.hpp"

void IRC_Server::handle_JOIN(int fd, std::vector<std::string> message)
{
	(void) fd;
    for (size_t i = 0; i< message.size(); ++i)
        std::cout << message[i] << ' ';

    std::cout << "\n" << std::endl;


    std::cout << "Handling JOIN command with parameters: " << message[0] << std::endl;
}
