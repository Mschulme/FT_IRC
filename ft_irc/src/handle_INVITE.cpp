#include "IRC_Server.hpp"

void IRC_Server::handle_INVITE(int fd, const std::vector<std::string> message)
{
    if (message.size() != 3) {
        sendClientMessage(INVITE_USAGE, fd);
        return;
    }

    std::string target_nickname = message[1];
    std::string channel_name = message[2].substr(1);

    std::vector<IRC_Channel>::iterator channel_it;
    for (channel_it = channelList.begin(); channel_it != channelList.end(); ++channel_it)
    {
        if (channel_it->getName() == channel_name)
        {
            break;
        }
    }

    if (channel_it == channelList.end())
    {
        return sendClientMessage(ERR_NOSUCHCHANNEL(clientList[fd].getNickname(), channel_it->getName()), fd);
    }

    IRC_Client target_client;
    bool found = false;
    for (size_t i = 0; i < clientList.size(); ++i)
    {
        if (clientList[i].getNickname() == target_nickname)
        {
            target_client = clientList[i]; 
            found = true; 
            break; 
        }
    }

    // Check if it is correct
    if (!found)
    {
        return sendClientMessage(ERR_NOSUCHNICK(clientList[fd].getNickname(), channel_it->getName()), fd);
    }

    channel_it->addMember(target_client);


    if (!channel_it->isMember(clientList[fd].getNickname()) || !channel_it->isOperator(clientList[fd].getNickname())) {
        sendClientMessage(ERR_CHANOPRIVSNEEDED(clientList[fd].getNickname(), channel_it->getName()) , fd);
        return;
    }

    std::map<int, IRC_Client>::iterator client_it;
    for (client_it = clientList.begin(); client_it != clientList.end(); ++client_it) {
        if (client_it->second.getNickname() == target_nickname) {
            break;
        }
    }

    if (client_it == clientList.end()) {
        sendClientMessage(ERR_NOSUCHNICK(clientList[fd].getNickname(), target_nickname), fd);
        return;
    }

    if (channel_it->getMembers().size() >= static_cast<size_t>(channel_it->getLimit())) {
        sendClientMessage("Channel " + channel_it->getName() + " is full", fd);
        return;
    }

    if (channel_it->isInvited(target_nickname)) {
        sendClientMessage(target_nickname + " is already invited to #" + channel_it->getName(), fd);
        return;
    }

    channel_it->addInvited(target_nickname);
    sendClientMessage("You invited " + target_nickname + " to #" + channel_name, fd);
    sendClientMessage(clientList[fd].getNickname() + " invited you to #" + channel_name, client_it->first);
}
