#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

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
	    std::map<int, class IRC_Client> clientList;
		std::vector<IRC_Channel> channelList;
		int setup(int port_number);
		int irc_server(int port_number, std::string password);

		void acceptNewClient(int sock, std::vector<pollfd> &pfds);
		void existingClient(std::vector<pollfd> &pfds, int i, std::string servPass);
		void compressClientList(int fd);

		void parser(std::string &message, int i, std::vector<pollfd> &pfds, std::string servPass);

    	IRC_Channel createChannel(std::string &name, int fd, std::map<int, IRC_Client> &clients);

		// Handling of received messages
		void eventHandler(std::vector<std::string> &incoming, int fd, std::string &pass);

        // Files for the different commands
		void handle_PASS(std::vector<std::string> &message, int fd, std::string &serverPass);
		void handle_NICK(int fd, std::vector<std::string> message);
		void handle_USER(int fd, std::vector<std::string> message);
		void handle_JOIN(int fd, std::vector<std::string> message);
		void handle_PRIVMSG(int fd, std::vector<std::string> message);
		void handle_INVITE(int fd, std::vector<std::string> message);
		void handle_KICK(int fd, std::vector<std::string> message);

};

#endif
