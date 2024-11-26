#include "IRC_Server.hpp"
#include "IRC_Channel.hpp"


void IRC_Server::handle_MODE(int fd, std::vector<std::string> message)
{
    if (message.size() < 3)
    {
        clientList[fd].reply(ERR_NEEDMOREPARAMS(clientList[fd].getNickname(), "MODE"), fd);
        return;
    }

    std::string channelName = message[1];
    std::string modeString = message[2];
    std::string parameter = (message.size() > 3) ? message[3] : "";

    /*
    IRC_Channel channel = getChannelByName(channelName);
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
                handleTopicRestrict(channel, add);
                break;
            case 'k':
                handleChannelKey(channel, add, parameter);
                break;
            case 'o':
                handleOperatorPrivilege(channel, add, parameter);
                break;
            case 'l':
                handleUserLimit(channel, add, parameter);
                break;
            default:
                clientList[fd].reply(ERR_UNKNOWNMODE(clientList[fd].getNickname(), std::string(1, modeString[i])), fd);
                break;
        }
    }
    */
}
