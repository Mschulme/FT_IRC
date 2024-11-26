#include "IRC_Server.hpp"
#include "IRC_Channel.hpp"

IRC_Channel::IRC_Channel(std::string& name, IRC_Client& firstclient) : _name(name), _isinviteOnly(true), _istopicRestricted(true){
	_clients.push_back(firstclient);
	_operators.push_back(firstclient);
	_hasPassword = false;
	_topic = "Default Topic";
	_channelKey = "";
	_hasLimit = true;
	_limit = 10;
}

IRC_Channel::~IRC_Channel() {}


std::string	IRC_Channel::getName()
{
	return (_name);
}


void IRC_Channel::setTopic(std::string& topic) {
	this->_topic = topic;
}


std::string IRC_Channel::getTopic() {
	return _topic;
}


void IRC_Channel::setTopicRestricted(bool topicRestricted) {
	this->_istopicRestricted = topicRestricted;
}


bool IRC_Channel::isTopicRestricted() {
	return _istopicRestricted;
}


void IRC_Channel::setInviteOnly(bool inviteOnly) {
	this->_isinviteOnly = inviteOnly;
}


bool IRC_Channel::isInviteOnly() {
	return _isinviteOnly;
}


bool	IRC_Channel::isInvited(std::string name)
{
	for (std::vector<std::string>::iterator it = _invited.begin(); it != _invited.end(); ++it) {
		if (name == *it)
			return (true);
	}
	return (false);
}


void IRC_Channel::setChannelKey(std::string& key) {
	this->_channelKey = key;
}


bool IRC_Channel::hasPassword() {
	return _hasPassword;
}


bool IRC_Channel::checkChannelKey(std::string& key) {
	return key == _channelKey;
}


std::string	IRC_Channel::getPassword()
{
	return (_channelKey);
}


void IRC_Channel::setOperator(IRC_Client& client) {
	_operators.push_back(client);
}


IRC_Client IRC_Channel::getClientByName(std::string clientName)
{
    IRC_Client client;
	IRC_Server server;

    for (std::map<int, IRC_Client>::iterator it = server.clientList.begin(); it != server.clientList.end(); ++it)
    {
        if (it->second.getNickname() == clientName)
        {
            client = it->second;
            break;
        }
    }
    return client;
}


void IRC_Channel::addOperator(std::string nickname)
{
	for (std::vector<IRC_Client>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
		if (it->getNickname() == nickname) {
			std::cout << nickname << " is already an operator of this Channel!" << std::endl;
			break;
		}
	}
	IRC_Client client = getClientByName(nickname);
	setOperator(client);
}


void IRC_Channel::addMember(IRC_Client client){
	_clients.push_back(client);
}


void IRC_Channel::addInvited(std::string &client)
{
	_invited.push_back(client);
}


void IRC_Channel::removeOperator(std::string nickname) {
	if(isOperator(nickname) == false){
		std::cout << nickname << " is not an operator of this Channel!" << std::endl;
		return;
	}
	for (std::vector<IRC_Client>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
		if (it->getNickname() == nickname) {
			_operators.erase(it);
			break;
		}
	}
	if(_operators.empty() && !_clients.empty()){
			setOperator(_clients[0]);
	}
}


bool IRC_Channel::isOperator(std::string nickname) {
	if(_operators.empty())
		return false;
	for (std::vector<IRC_Client>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
		if (it->getNickname() == nickname)
			return true;
	}
	return false;
}


void IRC_Channel::removeMember(std::string nickname) {
	if(isMember(nickname) == false){
		std::cout << nickname << " is not a client of this Channel!" << std::endl;
		return;
	}
	for (std::vector<IRC_Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getNickname() == nickname) {
			_clients.erase(it);
			break;
		}
	}
	if(isOperator(nickname))
		removeOperator(nickname);
}


bool IRC_Channel::isMember (std::string nickname) {
	if(_clients.empty())
		return false;
	for (std::vector<IRC_Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getNickname() == nickname) {
			return true;
		}
	}
	return false;
}


std::vector<IRC_Client> IRC_Channel::getMembers()
{
	return (_clients);
}


std::vector<IRC_Client> IRC_Channel::getOperators()
{
	return (_operators);
}


IRC_Client* IRC_Channel::findClient(std::string nickname){
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (nickname == _clients[i].getNickname())
			return (&_clients[i]);
	}
	return (NULL);
}


void IRC_Channel::setIsLimit(bool hasLimit){
	this->_hasLimit = hasLimit;
}


void IRC_Channel::setlimit(size_t limit){
	this->_limit = limit;
}


size_t IRC_Channel::getLimit(){
	return _limit;
}


void IRC_Channel::removeKey(void){
	_channelKey = "";
}


void IRC_Channel::removeUserLimit(void){
	_hasLimit = false;
}



/////

void IRC_Channel::handleInviteOnly(IRC_Channel& channel, bool add) {
    if (add) {
        channel.setInviteOnly(true);
    } else {
        channel.setInviteOnly(false);
    }
}

void IRC_Channel::handleTopicRestrict(IRC_Channel& channel, bool add) {
    if (add) {
        channel.setTopicRestricted(true);
    } else {
        channel.setTopicRestricted(false);
    }
}

void IRC_Channel::handleChannelKey(IRC_Channel& channel, bool add, std::string& key) {
    if (add) {
        channel.setChannelKey(key);
    } else {
        channel.removeKey();
    }
}

void IRC_Channel::handleOperatorPrivilege(IRC_Channel& channel, bool add, const std::string& user) {
    if (add) {
        channel.addOperator(user);
    } else {
        channel.removeOperator(user);
    }
}

void IRC_Channel::handleUserLimit(IRC_Channel& channel, bool add, const std::string& limit) {
    if (add) {
        channel.setlimit(std::atoi(limit.c_str()));
    } else {
        channel.removeUserLimit();
    }
}
