#ifndef IRC_CONNECTED_CLIENT_HPP
#define IRC_CONNECTED_CLIENT_HPP

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

class IRC_Connected_Client
{
	private:
		int client_fd;
		std::string IPadd;

	public:
        bool channel_operator;
		bool password_checked;
		std::string Username;
		std::string Nickname;

		IRC_Connected_Client(){};
		int GetFd(){return client_fd;};
		void SetFd(int fd){client_fd = fd;}
		void setIpAdd(std::string ipadd){IPadd = ipadd;}
};

#endif
