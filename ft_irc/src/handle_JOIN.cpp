#include "IRC_Server.hpp"

void IRC_Server::handle_JOIN(int fd, std::vector<std::string> message)
{
    std::string channelName;
    bool channelExists = false;

	if (clientList[fd].getAuthStatus())
    {
        channelName = message[1].substr(1, message[1].find(' ') - 1);
        std::string names;
        for (std::vector<IRC_Channel>::iterator it = channelList.begin(); it != channelList.end(); ++it)
        {
            if (it->getName() == channelName)
            {
                if (it->isInviteOnly() && !it->isInvited(clientList[fd].getNickname()))
                    return (sendClientMessage("Channel #" + it->getName() + " is invite only!", fd));
                if (it->hasPassword())
                {
                    if (message.size() != 3)
                        return (sendClientMessage(JOIN_USAGE, fd));
                    if (!it->checkChannelKey(message[2]))
                        return (sendClientMessage(ERR_PASSWDMISMATCH(clientList[fd].getNickname()), fd));
                }
                it->addMember(clientList[fd]);
                sendClientMessage(RPL_JOIN(clientList[fd].getNickname(), channelName), fd);
                for (size_t i = 0; i < it->getMembers().size(); i++)
                {
                    for (size_t j = 0; j < it->getOperators().size(); j++)
                    {
                        if (it->getOperators()[j].getNickname() == it->getMembers()[i].getNickname())
                            names += "@";
                    }
                    names += it->getMembers()[i].getNickname() + " ";
                }
                sendClientMessage(RPL_TOPIC(clientList[fd].getNickname(), channelName, it->getTopic()), fd);

                for (size_t i = 0; i < it->getMembers().size(); i++)
                {
                    sendClientMessage(RPL_NAMREPLY(it->getMembers()[i].getNickname(), channelName, names), it->getMembers()[i].getFd());
                    sendClientMessage(RPL_ENDOFNAMES(it->getMembers()[i].getNickname(), channelName), it->getMembers()[i].getFd());
                }
                std::cout << clientList[fd].getNickname() << " added to " << channelName << std::endl;
                channelExists = true;
                break;
            }
        }
        if (!channelExists)
        {
            IRC_Channel channel = createChannel(channelName, fd, clientList);

            sendClientMessage(RPL_JOIN(clientList[fd].getNickname(), channelName), fd);

            sendClientMessage(RPL_TOPIC(clientList[fd].getNickname(), channelName, channel.getTopic()), fd);
            
            sendClientMessage(RPL_NAMREPLY(clientList[fd].getNickname(), channelName, clientList[fd].getNickname()), fd);

            sendClientMessage(RPL_ENDOFNAMES(clientList[fd].getNickname(), channelName), fd);

            std::cout << clientList[fd].getNickname() << " added to " << channelName << std::endl;    
        }
    }
}
