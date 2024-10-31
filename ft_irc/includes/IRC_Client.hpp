#ifndef IRC_CLIENT_HPP
#define IRC_CLIENT_HPP

#include <iostream>
#include <sstream>
#include <string>
// Standard template libarary
#include <vector>
#include <map>
#include <list>
#include <set>

#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <poll.h>

#define MAXMSG 512
#define MAX_CLIENTS 1024

#include "IRC.hpp"


class IRC_Client
{
private:
	int 				  client_fd;
	bool				  _isAuthenticated;
	std::string 		  IPadd;
	std::string     	  _nickname;
    std::string     	  _username;
	std::string     	  _hostname;
	std::set<std::string> channels_member;
	std::set<std::string> channels_operator;

public:
	IRC_Client();
	IRC_Client(int fd);
	~IRC_Client();

	void	SetFd(int fd);
	int 	GetFd();

	void			set_nickname(std::string nickname);
	std::string     get_nickname();

	void	setAuthStatus(bool	status);
	bool 	getAuthStatus();
	
	void	setIpAdd(std::string ipadd){IPadd = ipadd;}

	void 		reply(std::string message, int fd);
	std::string	get_prefix();

};

#endif
