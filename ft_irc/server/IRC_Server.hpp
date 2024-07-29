#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

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

#include "IRC_Connected_Client.hpp"

#define MAXMSG 512
#define MAX_CLIENTS 1024


class IRC_Server
{
	private:
		int socket_fd;
		struct sockaddr_in client_address;
		
		// Check that the positions inside the vector match. Possible bug???
		// Contains a list of all clients
		std::vector<class IRC_Connected_Client> client_list;
		// Contains a list of the fd of the clients
		std::vector<struct pollfd> fds;
		// Set of the the Nicknames
		std::set<std::string> set_nicknames;

		struct Message
		{
			std::string prefix;
			std::string command;
			std::vector<std::string> parameters;
		};

		static void signal_handler_shutdown(int signum);
		void CloseFds(void);

	public:
		int irc_server(int port_number, std::string password);
		int setup(int port_number, std::string password);

		void AcceptNewClient(void);
		std::string ReceiveNewData(int fd);
		void CompressClientList(int fd);
		Message parser_irc_server(std::string received_message);
		bool isValidCommand(const std::string& command);
		bool isValidMiddle(const std::string& middle);
		bool isValidTrailing(const std::string& trailing);




		void EventHandler(int fd, std::string received_message, std::string password);

		// Commands-EventHandling

		void handle_PASS(int fd, Message message, std::string password);
		void handle_NICK(int fd, Message message);
		void handle_USER(int fd, Message message);
		void handle_JOIN(int fd, Message message);
		void handle_PRIVMSG(int fd, Message message);

		// Commands_Channel_Operator
		/*
		void handle_KICK(Message message);
		void handle_INVITE(Message message);
		void handle_TOPIC(Message message);
		void handle_MODE(Message message);
		*/
};

#endif