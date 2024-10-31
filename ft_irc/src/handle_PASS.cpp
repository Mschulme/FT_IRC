#include "IRC_Server.hpp"
#include "IRC.hpp"


void IRC_Server::handle_PASS(std::vector<std::string> &message, std::map<int, IRC_Client> &clients, int fd, std::string &serverPass)
{
    if (message.size() != 2)
		return (printInClient("Usage: </PASS> <PASSWORD>", fd));
	if (serverPass == message[1])
	{
		clients[fd].setAuthStatus(true);
		printInClient("Authentication Successful", fd);
	}
	else
	{
		printInClient("Incorrect Password", fd);
		return ;
	}
}
