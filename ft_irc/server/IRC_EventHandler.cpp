#include "IRC_Server.hpp"

void IRC_Server::EventHandler(std::vector<std::string> &incoming, std::map<int, IRC_Client> &clients, int fd, std::string &pass)
{
	std::string commands[] = {"JOIN", "PASS"};
	std::string filteredMsg = "";
	std::vector<std::string> message;

	for (std::vector<std::string>::iterator currentChar = incoming.begin(); currentChar != incoming.end(); ++currentChar)
	{
		std::string tempMsg;
		for (size_t i = 0; i < currentChar->length(); ++i) 
		{
			if (isprint((*currentChar)[i]))
				tempMsg += (*currentChar)[i];
		}
		filteredMsg += tempMsg;
		if (currentChar != incoming.end() - 1)
		{
			filteredMsg += " ";
		}
		message.push_back(tempMsg);
	}
	std::string receivedCommand = capitalize(message[0]);
	size_t i;

	for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
		if (receivedCommand == commands[i])
			break;
	switch (i)
	{
		case 0:
			handle_JOIN(fd, message);
			break;
		
		case 1:
			handle_PASS(message, clients, fd, pass);
			break;

		default:
			break;
		}
}
