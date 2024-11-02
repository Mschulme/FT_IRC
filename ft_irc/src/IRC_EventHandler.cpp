#include "IRC_Server.hpp"

void IRC_Server::eventHandler(std::vector<std::string> &rawMessage, int fd, std::string &pass)
{
	std::string commands[] = {"JOIN", "PASS", "NICK", "PRIVMSG", "INVITE"};

	std::vector<std::string> message = escapeRawMessage(rawMessage);

	if (message.size() > 0)
	{
		size_t 		i;

		for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
			if (toUpper(message[0]) == commands[i])
				break;

		switch (i)
		{
			case 0:
				handle_JOIN(fd, message);
				break;
			
			case 1:
				handle_PASS(message, fd, pass);
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
