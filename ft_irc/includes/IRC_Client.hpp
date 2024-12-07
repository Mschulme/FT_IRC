#ifndef IRC_CLIENT_HPP
#define IRC_CLIENT_HPP

#include <set>
#include <map>
#include <list>
#include <string>
#include <vector>

#include <poll.h>
#include <cerrno>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXMSG 512
#define MAX_CLIENTS 1024

#include "IRC.hpp"


class IRC_Client
{
	private:
		int 				  _clientFd;
		bool				  _isRegistered_NICK;
		bool				  _isRegistered_USER;
		bool				  _isAuthenticated;
		std::string 		  _ipAdd;
		std::string     	  _nickname;
		std::string     	  _username;
		std::string     	  _hostname;
		std::string			  _realname;

	public:
		IRC_Client();
		IRC_Client(int fd);
		~IRC_Client();

		void	setFd(int fd);
		int 	getFd();

		void			setNickname(std::string nickname);
		std::string     getNickname();

		void			setUsername(std::string nickname);
		std::string     getUsername();

		void			setRealname(std::string realname);
		std::string     getRealname();

		void	setAuthStatus(bool	status);
		bool 	getAuthStatus();
		
		void	setRegisteredStatus_NICK(bool	status);
		bool 	getRegisteredStatus_NICK();

		void	setRegisteredStatus_USER(bool	status);
		bool 	getRegisteredStatus_USER();

		void		setIpAdd(std::string ipadd){_ipAdd = ipadd;}

		void		reply(std::string message, int fd);
		std::string	getPrefix();

		void 		welcomeMessage();

};

#endif
