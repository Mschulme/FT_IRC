#include "IRC_Response.hpp"
#include "IRC_Client.hpp"
#include "IRC_Server.hpp"


void IRC_Server::handle_PASS(std::vector<std::string> &message, int fd, std::string &serverPass)
{
	// Implement the case, that the password starts with a colon.
	IRC_Client client = client_list[fd];

    if (message.size() == 1)
    {        
        return client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "PASS"), fd);
    }
	
	if (client.getAuthStatus() == true)
	{
		return client.reply(ERR_ALREADYREGISTERED(client.get_nickname()), fd);
	}

	if (serverPass != message[1])
	{
		return client.reply(ERR_PASSWDMISMATCH(client.get_nickname()), fd);
	}
	
	client_list[fd].setAuthStatus(true);
}
