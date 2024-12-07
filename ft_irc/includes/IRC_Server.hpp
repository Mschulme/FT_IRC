#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include <set>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <algorithm>

#include <poll.h>
#include <cerrno>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "IRC_Client.hpp"
#include "IRC_Channel.hpp"
#include "IRC_Response.hpp"

#define MAXMSG 512
#define MAX_CLIENTS 1024

class IRC_Channel;

class IRC_Server
{
	private:
		int _socketFD;
		struct sockaddr_in _clientAddress;
		
		std::vector<struct pollfd> _fds;
		typedef std::vector<pollfd>::iterator pollFdIterator;
       
		void closeFds(void);
		static void signalHandlerShutdown(int signum);

	public:
		int setup(int port_number);
		std::vector<IRC_Channel> channelList;
	    std::map<int, class IRC_Client> clientList;
		int irc_server(int port_number, std::string password);

		void compressClientList(int fd);
		void acceptNewClient(int sock, std::vector<pollfd> &pfds);
		void existingClient(std::vector<pollfd> &pfds, int i, const std::string &servPass);

		void parser(std::string &message, int i, std::vector<pollfd> &pfds, std::string servPass);

		IRC_Channel* getChannelByName(std::string channelName);
    	IRC_Channel createChannel(std::string &name, int fd, std::map<int, IRC_Client> &clients);

		void eventHandler(std::vector<std::string> &incoming, int fd, std::string &pass);

		void handle_NICK(int fd, std::vector<std::string> message);
		void handle_JOIN(int fd, std::vector<std::string> message);
		void handle_USER(int fd, std::vector<std::string> message);
		void handle_KICK(int fd, std::vector<std::string> message);
		void handle_PART(int fd, std::vector<std::string> message);
		void handle_MODE(int fd, std::vector<std::string> message);
		void handle_INVITE(int fd, std::vector<std::string> message);
		void handle_PRIVMSG(int fd, std::vector<std::string> message);
		void handle_PASS(std::vector<std::string> &message, int fd, const std::string &serverPass);
		void handle_TOPIC(int fd, std::vector<std::string> message);

};

#endif
