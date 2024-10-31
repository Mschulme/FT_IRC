#include "IRC_Server.hpp"

void IRC_Server::EventHandler(std::vector<std::string> &incoming, std::map<int, IRC_Client> &clients, int fd, std::string &pass)
{
	std::string filteredMsg = "";
	std::vector<std::string> message;
	std::string commands[] = {"JOIN", "PASS", "NICK", "PRIVMSG"};

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

	if (message.size() > 0)  // Fix for Segfault.
	{
		size_t 		i;
		std::string receivedCommand = capitalize(message[0]);

		std::cout << "reached_2" << std::endl;

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

			case 2:
				handle_NICK(fd, message);
				break;

			case 3:
				handle_PRIVMSG(fd, message);
				break;

			default:
				break;
		}
	}
}
