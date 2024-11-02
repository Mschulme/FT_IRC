#include "IRC_Response.hpp"
#include "IRC_Client.hpp"
#include "IRC_Server.hpp"


void IRC_Server::handle_PASS(std::vector<std::string> &message, int fd, std::string &serverPass)
{
	// Implement the case, that the password starts with a colon.
	IRC_Client client = clientList[fd];

    if (message.size() == 1)
    {        
        return client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "PASS"), fd);
    }
	
	if (client.getAuthStatus() == true)
	{
		return client.reply(ERR_ALREADYREGISTERED(client.getNickname()), fd);
	}

	if (serverPass != message[1])
	{
		return client.reply(ERR_PASSWDMISMATCH(client.getNickname()), fd);
	}
	
	clientList[fd].setAuthStatus(true);
}
