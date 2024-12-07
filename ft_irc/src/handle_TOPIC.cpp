#include "IRC_Server.hpp"
#include "IRC_Channel.hpp"

void IRC_Server::handle_TOPIC(int fd, std::vector<std::string> message)
{
    if (message.size() < 2)
    {
        clientList[fd].reply("461 TOPIC :Not enough parameters", fd);
        return;
    }

    std::string channelName = message[1];
    std::string newTopic = (message.size() > 2) ? message[2] : "";

    if (channelName.empty() || channelName[0] != '#')
    {
        return sendClientMessage("Error: Channel name must start with '#'", fd);
    }

    channelName = channelName.substr(1);

    IRC_Channel* channel = getChannelByName(channelName);
    if (channel->getName() != channelName) 
    {
        clientList[fd].reply("403 " + channelName + " :No such channel", fd);
        return;
    }

    IRC_Client client = clientList[fd];
    if (!channel->isMember(client.getNickname()))
    {
        client.reply("442 " + channelName + " :You're not on that channel", fd);
        return;
    }

    if (newTopic.empty())
    {
        if (channel->getTopic().empty())
        {
            client.reply("331 " + channelName + " :No topic is set", fd);
        }
        else
        {
            client.reply("332 " + channelName + " :" + channel->getTopic(), fd);
        }
        return;
    }

    if (!channel->isOperator(client.getNickname()) && channel->isTopicRestricted())
    {
        client.reply("482 " + channelName + " :You're not channel operator", fd);
        return;
    }

    channel->setTopic(newTopic);
    channel->broadcastMessage(":" + client.getNickname() + " TOPIC " + channelName + " :" + newTopic);
}
