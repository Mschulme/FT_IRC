#ifndef IRC_CLIENT_HPP
#define IRC_CLIENT_HPP

#include <iostream>
#include <sstream>
#include <string>
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
		int 				  _clientFd;
		bool				  _isAuthenticated;
		std::string 		  _ipAdd;
		std::string     	  _nickname;
		std::string     	  _username;
		std::string     	  _hostname;

	public:
		IRC_Client();
		IRC_Client(int fd);
		~IRC_Client();

		void	setFd(int fd);
		int 	getFd();

		void			setNickname(std::string nickname);
		std::string     getNickname();

		void	setAuthStatus(bool	status);
		bool 	getAuthStatus();
		
		void	setIpAdd(std::string ipadd){_ipAdd = ipadd;}

		void 		reply(std::string message, int fd);
		std::string	getPrefix();

		void 		welcomeMessage();

};

#endif
