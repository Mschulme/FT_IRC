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

#include "IRC_Client.hpp"

#define MAXMSG 512
#define MAX_CLIENTS 1024


class IRC_Server
{
	private:
		int socket_fd;
		struct sockaddr_in client_address;
		
		std::vector<struct pollfd> fds;
		typedef std::vector<pollfd>::iterator pollFdIterator;
        std::map<int, class IRC_Client> client_list;

        // Set of Usernames to check for uniqueness.

		struct Message
		{
			std::string prefix;
			std::string command;
			std::vector<std::string> parameters;
		};

		void CloseFds(void);
		static void signal_handler_shutdown(int signum);

	public:
		int setup(int port_number);
		int irc_server(int port_number, std::string password);

		void AcceptNewClient(int sock, std::vector<pollfd> &pfds);
		void ExistingClient(std::vector<pollfd> &pfds, int i, std::string servPass);
		void CompressClientList(int fd);
		std::string ReceiveNewData(int fd);

		void parser_irc_server(std::string &message, int i, std::vector<pollfd> &pfds, std::string servPass);
		bool isValidCommand(const std::string& command);
		bool isValidMiddle(const std::string& middle);
		bool isValidTrailing(const std::string& trailing);
		void validateInput(std::string port, std::string password);

		// Handling of received messages
		void EventHandler(std::vector<std::string> &incoming, int fd, std::string &pass);

        // Files for the different commands
		void handle_PASS(std::vector<std::string> &message, int fd, std::string &serverPass);
		void handle_NICK(int fd, std::vector<std::string> message);
		void handle_USER(int fd, std::vector<std::string> message);
		void handle_JOIN(int fd, std::vector<std::string> message);
		void handle_PRIVMSG(int fd, std::vector<std::string> message);

};

#endif
