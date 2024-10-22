#include "IRC_Server.hpp"

void IRC_Server::handle_PRIVMSG(int fd, Message message)
{
    /*
        Iterate through the std::map<int, class IRC_Connected_Client> client_list; client list. If the username matches, forward the message via
            send(fd, message.c_str(), message.size(), 0); to the receiver.

        The function depends on the parser. If the parser is correct, this should be straight forward.
    */

	(void) fd;
    std::cout << "Handling PRIVMSG command with parameters: " << message.parameters[0] << std::endl;
}
