#include "IRC_Client.hpp"


IRC_Client::IRC_Client() {}

IRC_Client::IRC_Client(int fd) : _clientFd(fd)
{
    _isAuthenticated = false;
	_nickname = "Default";
	_username = "Default";
    _hostname = "Localhost";
}


void	IRC_Client::setFd(int fd)
{
    _clientFd = fd;
}

int 	IRC_Client::getFd(void)
{
    return _clientFd;
}


bool    IRC_Client::getAuthStatus()
{
    return (_isAuthenticated);
}

void    IRC_Client::setAuthStatus(bool status)
{
    _isAuthenticated = status;
}

void     IRC_Client::setNickname(std::string nickname) 
{ 
    _nickname = nickname; 
}

std::string     IRC_Client::getNickname() 
{ 
    return _nickname; 
}

void IRC_Client::reply(std::string message, int fd)
{
    message = message + "\n";
    if (send(fd, message.c_str(), message.length(), 0) < 0)
        return ;
}

std::string     IRC_Client::getPrefix() 
{
    std::string username = _username.empty() ? "" : "!" + _username;
    std::string hostname = _hostname.empty() ? "" : "@" + _hostname;

    return _nickname + username + hostname;
}

void IRC_Client::welcomeMessage()
{
    sendClientMessage("Welcome to the server", _clientFd);
}

IRC_Client::~IRC_Client() {}
