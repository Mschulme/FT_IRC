#include "IRC_Response.hpp"
#include "IRC_Client.hpp"
#include "IRC_Server.hpp"


void IRC_Server::handle_PASS(std::vector<std::string> &message, int fd, std::string &serverPass)
{
	// Implement the case, that the password starts with a colon.
    if (message.size() == 1)
    {      
        return clientList[fd].reply(ERR_NEEDMOREPARAMS(clientList[fd].getNickname(), "PASS"), fd);
    }
	if (clientList[fd].getAuthStatus() == true)
	{
		return clientList[fd].reply(ERR_ALREADYREGISTERED(clientList[fd].getNickname()), fd);
	}

	if (serverPass != message[1])
	{
		return clientList[fd].reply(ERR_PASSWDMISMATCH(clientList[fd].getNickname()), fd);
	}
	
	clientList[fd].setAuthStatus(true);
}
