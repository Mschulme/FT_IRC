#include "IRC_Server.hpp"
#include "IRC.hpp"


void IRC_Server::parser_irc_server(std::string &message, std::map<int, IRC_Client> &clients, int i, std::vector<pollfd> &pfds, std::string servPass)
{
	int clientFd = pfds[i].fd;
	std::vector<std::string> split = ft_split(message);
	IRC_Server::EventHandler(split, clients, clientFd, servPass);
}


bool IRC_Server::isValidCommand(const std::string& command) 
{
	if (command.empty()) return false;
	if (std::isalpha(command[0])) {
		for (size_t i = 1; i < command.size(); ++i) {
			if (!std::isalpha(command[i])) return false;
		}
		return true;
	} else if (std::isdigit(command[0])) {
		if (command.size() != 3) return false;
		for (size_t i = 1; i < command.size(); ++i) {
			if (!std::isdigit(command[i])) return false;
		}
		return true;
	}
	return false;
}


bool IRC_Server::isValidMiddle(const std::string& middle)
{
	for (size_t i = 0; i < middle.size(); ++i)
	{
		if (middle[i] == '\0' || middle[i] == ' ' || middle[i] == ':')
		{
			return false;
		}
	}
	return true;
}


bool IRC_Server::isValidTrailing(const std::string& trailing)
{
	for (size_t i = 0; i < trailing.size(); ++i)
	{
		if (trailing[i] == '\0' || trailing[i] == '\r' || trailing[i] == '\n')
		{
			return false;
		}
	}
	return true;
}
