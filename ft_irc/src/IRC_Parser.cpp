#include "IRC_Server.hpp"
#include "IRC.hpp"

void IRC_Server::parser(std::string &message, int i, std::vector<pollfd> &pfds, std::string servPass)
{
	int clientFd = pfds[i].fd;
	std::vector<std::string> split = ft_split(message);
	IRC_Server::eventHandler(split, clientFd, servPass);
}

bool    isPortNumeric(std::string port)
{
    for (size_t i = 0; i < port.length(); i++)
        if (!isdigit(port[i]))
            return (false);
    return (true);
}

bool    getPortLength(std::string port)
{
    if (port.length() != 4)
        return (false);
    return (true);
}

bool    getPasswordLength(std::string password)
{
    if (password.length() < 4 || password.length() > 8)
        return (false);
    return (true);
}

void    validateInput(std::string port, std::string password)
{
    if (!isPortNumeric(port))
        throw (std::runtime_error("Port argument should only contain digits!"));
    if (!getPortLength(port))
        throw (std::runtime_error("Port argument should contain 4 digits!"));
    if (!getPasswordLength(password))
        throw (std::runtime_error("Password argument should be between 4 and 8 characters long!"));
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

std::string capitalize(std::string command)
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

