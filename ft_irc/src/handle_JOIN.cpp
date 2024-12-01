#include "IRC_Server.hpp"

bool isValidChannelName(const std::string& channel_name);

void IRC_Server::handle_JOIN(int fd, std::vector<std::string> message)
{
    std::string channelName;
    bool channelExists = false;

	if (clientList[fd].getAuthStatus())
    {

        if (isValidChannelName(message[1]) == false)
            return (sendClientMessage(ERR_BADCHANMASK(clientList[fd].getNickname()), fd));

        channelName = message[1].substr(1, message[1].find(' ') - 1);
        std::string names;
        for (std::vector<IRC_Channel>::iterator it = channelList.begin(); it != channelList.end(); ++it)
        {
            if (it->getName() == channelName)
            {
                if (it->isMember(clientList[fd].getNickname()))
                {
                    clientList[fd].reply(clientList[fd].getNickname() + " is already on channel #" + channelName, fd);
                    return;
                }

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
            channel.addMember(clientList[fd]);

            sendClientMessage(RPL_JOIN(clientList[fd].getNickname(), channelName), fd);

            sendClientMessage(RPL_TOPIC(clientList[fd].getNickname(), channelName, channel.getTopic()), fd);
            
            sendClientMessage(RPL_NAMREPLY(clientList[fd].getNickname(), channelName, clientList[fd].getNickname()), fd);

            sendClientMessage(RPL_ENDOFNAMES(clientList[fd].getNickname(), channelName), fd);

            std::cout << clientList[fd].getNickname() << " added to " << channelName << std::endl;    
        }
    }
}


bool isValidChannelName(const std::string& channel_name)
{
    // Channel name must start with a valid prefix character (#, &, !, +)
    if (channel_name.empty() || (channel_name[0] != '#' && channel_name[0] != '&' && channel_name[0] != '!' && channel_name[0] != '+')) {
        return false;
    }

    // Maximum length is usually 50 characters (you can adjust this if needed)
    const size_t MAX_LENGTH = 50;
    if (channel_name.length() > MAX_LENGTH) {
        return false;
    }

    // Check each character in the channel name
    for (std::string::const_iterator it = channel_name.begin() + 1; it != channel_name.end(); ++it) {
        char ch = *it;
        if (!std::isalnum(ch) && ch != '-' && ch != '_' && ch != '.') {
            // If character is not alphanumeric and not a valid special character, return false
            return false;
        }
    }

    return true;
}
