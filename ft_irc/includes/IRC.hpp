#ifndef IRC_HPP
# define IRC_HPP

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

#include "IRC_Client.hpp"
#include "IRC_Server.hpp"

#define SERVER_IP "127.0.0.1"


std::string                 toUpper(std::string command);
std::vector<std::string>    ft_split(std::string message);
void                        sendClientMessage(std::string message, int fd);
void                        validateInput(std::string port, std::string password);
std::vector<std::string>    escapeRawMessage(std::vector<std::string> &rawMessage);
std::string                 get_prefix(std::string clientName, std::string channelName, std::string code);

#endif
