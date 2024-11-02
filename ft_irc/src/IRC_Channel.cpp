#include "IRC_Channel.hpp"

IRC_Channel::IRC_Channel(std::string& name, IRC_Client& firstclient) : _name(name), _isinviteOnly(true), _istopicRestricted(true){
	_clients.push_back(firstclient);
	_operators.push_back(firstclient);
	_hasPassword = false;
	_topic = "Default Topic";
	_channelKey = "";
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

void IRC_Channel::setOperatorPrivileges(IRC_Client& client) {
	_operators.push_back(client);
}

void IRC_Channel::addMember(IRC_Client client){
	_clients.push_back(client);
}

void IRC_Channel::addInvited(std::string &client)
{
	_invited.push_back(client);
}

void IRC_Channel::removeOperatorPrivileges(std::string& nickname) {
	if(hasOperatorPrivileges(nickname) == false){
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
			setOperatorPrivileges(_clients[0]);
	}
}

bool IRC_Channel::hasOperatorPrivileges(std::string& nickname) {
	if(_operators.empty())
		return false;
	for (std::vector<IRC_Client>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
		if (it->getNickname() == nickname)
			return true;
	}
	return false;
}

void IRC_Channel::removeMember(std::string& nickname) {
	if(isclient(nickname) == false){
		std::cout << nickname << " is not a client of this Channel!" << std::endl;
		return;
	}
	for (std::vector<IRC_Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getNickname() == nickname) {
			_clients.erase(it);
			break;
		}
	}
	if(hasOperatorPrivileges(nickname))
		removeOperatorPrivileges(nickname);
}

bool IRC_Channel::isclient (std::string& nickname) {
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

IRC_Client* IRC_Channel::findClient(std::string& nickname){
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (nickname == _clients[i].getNickname())
			return (&_clients[i]);
	}
	return (NULL);
}