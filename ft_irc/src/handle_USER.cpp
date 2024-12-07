#include "IRC_Server.hpp"


std::string extract_Username(const std::vector<std::string>& message);
std::string extract_Realname(const std::vector<std::string>& message);


void IRC_Server::handle_USER(int fd, std::vector<std::string> message)
{
	std::string username = extract_Username(message);
	std::string realname = extract_Realname(message);

	if (username.empty() or realname.empty())
		return clientList[fd].reply(ERR_NEEDMOREPARAMS(clientList[fd].getNickname(), "USER"), fd);
	else if (clientList[fd].getRegisteredStatus_USER() == true)
	{
		return clientList[fd].reply(ERR_ALREADYREGISTERED(clientList[fd].getNickname()), fd);
	}
	else
	{
		clientList[fd].setUsername(username);
		clientList[fd].setRealname(realname);
		clientList[fd].setRegisteredStatus_USER(true);
	}
	return ;
}


std::string extract_Username(const std::vector<std::string> &message)
{
	if (message.size() > 1)
		return message[1];
	return "";
}

std::string extract_Realname(const std::vector<std::string> &message)
{
	std::string realname = "";
	if (message.size() > 4)
	{
		realname = message[4];
		for (size_t i = 5; i < message.size(); ++i)
			realname += " " + message[i]; 
		if (!realname.empty() && realname[0] == ':')
			realname.erase(0, 1);
		return realname;
	}
	return "";
}
