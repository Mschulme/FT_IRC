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
		
		std::vector<struct pollfd> fds;
        std::map<int, class IRC_Connected_Client> client_list;

		struct Message
		{
			std::string prefix;
			std::string command;
			std::vector<std::string> parameters;
		};

		void CloseFds(void);
		static void signal_handler_shutdown(int signum);

	public:
		int setup(int port_number, std::string password);
		int irc_server(int port_number, std::string password);

		void AcceptNewClient(void);
		void CompressClientList(int fd);
		std::string ReceiveNewData(int fd);

		Message parser_irc_server(std::string received_message);
		bool isValidCommand(const std::string& command);
		bool isValidMiddle(const std::string& middle);
		bool isValidTrailing(const std::string& trailing);

		// Handling of received messages
		void EventHandler(int fd, std::string received_message, std::string password);

        // Files for the different commands
		void handle_PASS(int fd, const Message& message, const std::string& password);
		void handle_NICK(int fd, Message message);
		void handle_USER(int fd, Message message);
		void handle_JOIN(int fd, Message message);
		void handle_PRIVMSG(int fd, Message message);

};

#endif