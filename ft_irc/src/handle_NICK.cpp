#include "IRC_Server.hpp"

static bool Forbidden_Character(const std::string& str);

void IRC_Server::handle_NICK(int fd, std::vector<std::string> message)
{
    std::string oldNickName;

	IRC_Client client = clientList[fd];
    
    if (message.size() == 1)
    {        
        return client.reply(ERR_NONICKNAMEGIVEN(client.getNickname()), fd);
    }
    
    std::string nickname = message[1];
    if (Forbidden_Character(nickname) == true)
        return client.reply(ERR_ERRONEUSNICKNAME(client.getNickname()), fd);

    for (size_t i = 0; i < clientList.size(); ++i)
    {
        if (i != (size_t)fd)
        {
            if (clientList[i].getNickname() == nickname)
            {
                return client.reply(ERR_NICKNAMEINUSE(client.getNickname()), fd);       
            }
        }
    }
    if (clientList[fd].getNickname() == "Default")
        client.welcomeMessage();
    oldNickName = clientList[fd].getNickname();
    clientList[fd].setNickname(nickname);
    
    for (std::vector<IRC_Channel>::iterator channelIt = channelList.begin(); channelIt != channelList.end(); ++channelIt)
    {
        for (std::vector<IRC_Client>::iterator it = channelIt->_clients.begin(); it != channelIt->_clients.end(); ++it)
        {
            if (it->getNickname() == oldNickName)
            {
                it->setNickname(nickname);
                break;
            }
        }
    }
}


static bool Forbidden_Character(const std::string& str)
{
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        if (*it == ' ' || *it == ',' || *it == '*' || *it == '?' || *it == '!' || *it == '@' || *it == '.')
            return true;
    }
    return false;
}
