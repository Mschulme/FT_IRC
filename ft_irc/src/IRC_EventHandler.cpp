#include "IRC_Server.hpp"

void IRC_Server::eventHandler(std::vector<std::string> &rawMessage, int fd, std::string &pass)
{
	std::string commands[] = {"PASS", "JOIN", "NICK", "PRIVMSG", "INVITE"};

	std::vector<std::string> message = escapeRawMessage(rawMessage);

	if (message.size() > 0)
	{
		size_t 		i;

		for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
			if (toUpper(message[0]) == commands[i])
				break;

		if (i == 0)
		{
			handle_PASS(message, fd, pass);
		}
		else
		{
			if (clientList[fd].getAuthStatus() != true)
			{
				return clientList[fd].reply(ERR_NOTREGISTERED(clientList[fd].getNickname()), fd);
			}
			switch (i)
			{
				case 1:
					handle_JOIN(fd, message);
					break;
				
				case 2:
					handle_NICK(fd, message);
					break;

				case 3:
					handle_PRIVMSG(fd, message);
					break;

				case 4:
					handle_INVITE(fd, message);
					break;

				default:
					break;
			}
		}
	}
}
