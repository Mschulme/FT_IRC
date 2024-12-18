#include "IRC_Channel.hpp"
#include "IRC_Server.hpp"

IRC_Channel::IRC_Channel(std::string& name, IRC_Client& firstclient) : _name(name), _isinviteOnly(true), _istopicRestricted(true){
	_clients.push_back(firstclient);
	_operators.push_back(firstclient);
	_hasPassword = false;
	_topic = "Default Topic";
	_channelKey = "";
	_hasLimit = true;
	_limit = 10;
}

IRC_Channel::IRC_Channel() {}

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

void IRC_Channel::SetHasPassword(bool flag) {
	_hasPassword = flag;
}

void IRC_Channel::removeKey() {
	_hasPassword = false;
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

void IRC_Channel::addMember(IRC_Client client)
{
	_clients.push_back(client);
}

void IRC_Channel::addInvited(std::string &client)
{
	_invited.push_back(client);
}

void IRC_Channel::removeOperator(std::string nickname) {
	if(isOperator(nickname) == false){
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
			_invited.erase(std::remove(_invited.begin(), _invited.end(), nickname), _invited.end());
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

void IRC_Channel::removeUserLimit(){
	_hasLimit = false;
}

size_t IRC_Channel::getLimit(){
	return _limit;
}

/////

void IRC_Channel::handleInviteOnly(IRC_Channel* channel, bool add) {
    if (add) {
        channel->setInviteOnly(true);
		channel->broadcastMessage("Channel mode is updated to Invite only");
    } else {
        channel->setInviteOnly(false);
		channel->broadcastMessage("Channel mode Invite only is removed");
    }
}

void IRC_Channel::handleTopicRestrict(IRC_Channel* channel, bool add) {
    if (add) {
		channel->broadcastMessage("Channel mode is updated to Topic restricted");
        channel->setTopicRestricted(true);
    } else {
        channel->setTopicRestricted(false);
		channel->broadcastMessage("Channel mode Topic restricted is removed");
    }
}

void IRC_Channel::handleChannelKey(IRC_Channel* channel, bool add, std::string& key) {
    if (add) {
        channel->setChannelKey(key);
		channel->SetHasPassword(true);
		channel->broadcastMessage("Channel mode password protection is added");
    } else {
        channel->removeKey();
		channel->SetHasPassword(false);
		channel->broadcastMessage("Channel mode password protection is removed");
    }
}

void IRC_Channel::handleOperatorPrivilege(IRC_Channel* channel, bool add, const std::string& user) {
    if (add) {
        channel->addOperator(user);
		channel->broadcastMessage(user + " is operator now");
    } else {
        channel->removeOperator(user);
		channel->broadcastMessage(user + " is not operator anymore");
    }
}

void IRC_Channel::handleUserLimit(IRC_Channel* channel, bool add, const std::string& limit) {
    if (add) {
		if (std::atoi(limit.c_str()) > 0)
		{
			channel->setlimit(std::atoi(limit.c_str()));
			channel->broadcastMessage("Channel mode member limit is set to " + limit);
		}
    } 
	else {
        channel->removeUserLimit();
		channel->broadcastMessage("Channel mode member limit is removed ");
    }
}

void IRC_Channel::broadcastMessage(std::string message)
{
    for (std::vector<IRC_Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        (it)->reply(message, it->getFd());
    }
}
