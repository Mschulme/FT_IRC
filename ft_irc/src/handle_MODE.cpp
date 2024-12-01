#include "IRC_Server.hpp"
#include "IRC_Channel.hpp"


void IRC_Server::handle_MODE(int fd, std::vector<std::string> message)
{
    IRC_Server server;

    if (message.size() < 2)
    {
        clientList[fd].reply(ERR_NEEDMOREPARAMS(clientList[fd].getNickname(), "MODE"), fd);
        return;
    }

    std::string channelName = message[1];
    std::string modeString = (message.size() > 2) ? message[2] : "";
    std::string parameter = (message.size() > 3) ? message[3] : "";

    if (channelName.empty() || channelName[0] != '#')
    {
        return sendClientMessage("Error: Channel name must start with '#'", fd);
    }

    channelName = channelName.substr(1);
    
    IRC_Channel channel = getChannelByName(channelName);
    if (channel.getName() != channelName) 
    {
        clientList[fd].reply("403 " + channelName + " :No such channel", fd);
        return;
    }

    if (!channel.isOperator(clientList[fd].getNickname()))
    {
        return sendClientMessage("You should be operator to change mode", fd);
    }
    else
    {
        bool add = true;
        for (std::string::size_type i = 0; i < modeString.size(); ++i)
        {
            switch (modeString[i])
            {
                case '+':
                    add = true;
                    break;
                case '-':
                    add = false;
                    break;
                case 'i':
                    channel.handleInviteOnly(channel, add);
                    break;
                case 't':
                    channel.handleTopicRestrict(channel, add);
                    break;
                case 'k':
                    channel.handleChannelKey(channel, add, parameter);
                    break;
                case 'o':
                    channel.handleOperatorPrivilege(channel, add, parameter);
                    break;
                case 'l':
                    channel.handleUserLimit(channel, add, parameter);
                    break;
                default:
                    clientList[fd].reply(ERR_UNKNOWNMODE(clientList[fd].getNickname(), std::string(1, modeString[i])), fd);
                    break;
            }
        }
    }
}

IRC_Channel IRC_Server::getChannelByName(std::string channelName)
{
    IRC_Channel channel;
    for (std::vector<IRC_Channel>::iterator it = channelList.begin(); it != channelList.end(); ++it)
    {
        if (it->getName() == channelName)
        {
            channel = *it;
            break;
        }
    }
    return channel;
}
