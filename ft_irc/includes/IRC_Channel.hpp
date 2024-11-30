#ifndef IRC_CHANNEL_HPP
#define IRC_CHANNEL_HPP


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
        bool       _hasLimit;
        size_t	   _limit;


    public:
        IRC_Channel(std::string& name, IRC_Client& firstMember);
        IRC_Channel();
        ~IRC_Channel();


        void removeKey(void);
        void removeUserLimit(void);
        void setTopic(std::string& topic);
        void setInviteOnly(bool inviteOnly);
        void setOperator(IRC_Client& client);
        void setChannelKey(std::string& key);
        void addOperator(std::string nickname);
        void setTopicRestricted(bool topicRestricted);


        std::string getName();
        std::string getTopic();
        std::string	getPassword();
        std::vector<IRC_Client > getMembers();
        std::vector<IRC_Client > getOperators();
        IRC_Client getClientByName(std::string clientName);


        bool isTopicRestricted();
        bool isInviteOnly();
        bool isInvited(std::string name);
        bool hasPassword();
        bool checkChannelKey(std::string& key);
        void addMember(IRC_Client client);
        void addInvited(std::string &client);
        void removeOperator(std::string nickname);
        bool isOperator(std::string nickname);
        void removeMember(std::string nickname);
        bool isMember (std::string nickname);
        IRC_Client* findClient(std::string nickname);
        void setIsLimit(bool islimit);
        void setlimit(size_t limit);
        size_t getLimit();


        void handleInviteOnly(IRC_Channel& channel, bool add);
        void handleTopicRestrict(IRC_Channel& channel, bool add);
        void handleChannelKey(IRC_Channel& channel, bool add, std::string& key);
        void handleOperatorPrivilege(IRC_Channel& channel, bool add, const std::string& user);
        void handleUserLimit(IRC_Channel& channel, bool add, const std::string& limit);
        void broadcastMessage(std::string message);

};

#endif