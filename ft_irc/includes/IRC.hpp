#ifndef IRC_HPP
# define IRC_HPP

# include <string>
# include <stdbool.h>
# include <sstream>
# include <iostream>
# include <vector>
# include <stdexcept>
# include <unistd.h>
# include <poll.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <signal.h>
# include <cstring>
# include <stdlib.h>
# include <algorithm>
# include <err.h>
# include <map>
# include <cerrno>
# include "IRC_Client.hpp"
# include "IRC_Server.hpp"

#define SERVER_IP "127.0.0.1"


std::string                 toUpper(std::string command);
void                        sendClientMessage(std::string message, int fd);
std::vector<std::string>    ft_split(std::string message);
void                        validateInput(std::string port, std::string password);
std::string                 get_prefix(std::string clientName, std::string channelName, std::string code);
std::vector<std::string>    escapeRawMessage(std::vector<std::string> &rawMessage);

#endif
