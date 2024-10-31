#include "IRC_Client.hpp"


IRC_Client::IRC_Client() {}

IRC_Client::IRC_Client(int fd) : client_fd(fd)
{
    _isAuthenticated = false;
	_nickname = "Default";
	_username = "Default";
}

bool    IRC_Client::getAuthStatus()
{
    return (_isAuthenticated);
}

void    IRC_Client::setAuthStatus(bool status)
{
    _isAuthenticated = status;
}

IRC_Client::~IRC_Client() {}
