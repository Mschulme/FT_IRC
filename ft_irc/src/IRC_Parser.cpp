#include "IRC.hpp"
#include "IRC_Server.hpp"

void IRC_Server::parser(std::string &message, int i, std::vector<pollfd> &pfds, std::string servPass)
{
	int clientFd = pfds[i].fd;
	std::vector<std::string> split = ft_split(message);
	IRC_Server::eventHandler(split, clientFd, servPass);
}


std::vector<std::string> ft_split(std::string message)
{
    std::vector<std::string> split;
    std::stringstream ss(message);
    std::string token;

    while (ss >> token)
        split.push_back(token);
    return (split);
}


void sendClientMessage(std::string message, int fd)
{
    message = message + "\n";
    if (send(fd, message.c_str(), message.length(), 0) < 0)
        return ;
}


std::string toUpper(std::string command)
{
    std::string result;
    for (size_t i = 0; i < command.length(); i++)
        result += toupper(command[i]);
    return (result);
}


std::string get_prefix(std::string clientName, std::string channelName, std::string code) 
{
    return std::string(":") + SERVER_IP + code + clientName + " = #" + channelName + " :@";
}


std::vector<std::string> escapeRawMessage(std::vector<std::string> &rawMessage)
{
    std::string escapedMessage = "";
    std::vector<std::string> message;

    for (std::vector<std::string>::iterator currentChar = rawMessage.begin(); currentChar != rawMessage.end(); ++currentChar)
	{
		std::string tempMsg;
		for (size_t i = 0; i < currentChar->length(); ++i) 
		{
			if (isprint((*currentChar)[i]))
				tempMsg += (*currentChar)[i];
		}
		escapedMessage += tempMsg;
		if (currentChar != rawMessage.end() - 1)
		{
			escapedMessage += " ";
		}
		message.push_back(tempMsg);
	}
    return message;
}
