#ifndef IRC_CHANNEL_HPP
#define IRC_CHANNEL_HPP


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

class IRC_Channel 
{
    private:
        std::string     _name;
        std::string     _topic;
        std::set<int>   _clients;
        std::set<int>   _operators;

    public:
        IRC_Channel();
        ~IRC_Channel();

};

#endif