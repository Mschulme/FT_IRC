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

#include "../includes/IRC_Client.hpp"

class IRC_Client;

class IRC_Channel 
{
    private:
        std::string     _name;
        std::string     _topic;
        std::string     _channelKey;
        std::vector<IRC_Client>   _clients;
        std::vector<IRC_Client>   _operators;
        std::vector<std::string > _invited;
        bool       _hasPassword;
        bool       _isinviteOnly;
        bool       _istopicRestricted;


    public:
        IRC_Channel(std::string& name, IRC_Client& firstMember);
        IRC_Channel();
        ~IRC_Channel();

        void setTopic(std::string& topic);
        void setTopicRestricted(bool topicRestricted);
        void setInviteOnly(bool inviteOnly);
        void setChannelKey(std::string& key);
        void setOperatorPrivileges(IRC_Client& client);

        std::string getName();
        std::string getTopic();
        std::string	getPassword();
        std::vector<IRC_Client > getMembers();
        std::vector<IRC_Client > getOperators();

        bool isTopicRestricted();
        bool isInviteOnly();
        bool isInvited(std::string name);
        bool hasPassword();
        bool checkChannelKey(std::string& key);
        void addMember(IRC_Client client);
        void addInvited(std::string &client);
        void removeOperatorPrivileges(std::string& nickname);
        bool hasOperatorPrivileges(std::string& nickname);
        void removeMember(std::string& nickname);
        bool isclient (std::string& nickname);
        IRC_Client* findClient(std::string& nickname);

};

#endif