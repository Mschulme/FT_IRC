#include "IRC_Client.hpp"


IRC_Client::IRC_Client() {}

IRC_Client::IRC_Client(int fd) : client_fd(fd)
{
    _isAuthenticated = false;
	_nickname = "Default";
	_username = "Default";
    _hostname = "Localhost";
}


void	IRC_Client::SetFd(int fd)
{
    client_fd = fd;
}

int 	IRC_Client::GetFd(void)
{
    return client_fd;
}


bool    IRC_Client::getAuthStatus()
{
    return (_isAuthenticated);
}

void    IRC_Client::setAuthStatus(bool status)
{
    _isAuthenticated = status;
}

void     IRC_Client::set_nickname(std::string nickname) 
{ 
    _nickname = nickname; 
}

std::string     IRC_Client::get_nickname() 
{ 
    return _nickname; 
}

void IRC_Client::reply(std::string message, int fd)
{
    message = message + "\n";
    if (send(fd, message.c_str(), message.length(), 0) < 0)
        return ;
}

std::string     IRC_Client::get_prefix() 
{
    std::string username = _username.empty() ? "" : "!" + _username;
    std::string hostname = _hostname.empty() ? "" : "@" + _hostname;

    return _nickname + username + hostname;
}

IRC_Client::~IRC_Client() {}
