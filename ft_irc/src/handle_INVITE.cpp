#include "IRC_Server.hpp"

void IRC_Server::handle_INVITE(int fd, std::vector<std::string> message)
{
    if (message.size() != 3) {
        sendClientMessage(INVITE_USAGE, fd);
        return;
    }

    std::string target_nickname = message[1];
    std::string channel_name = message[2];

    if (channel_name.empty() || channel_name[0] != '#')
    {
        return sendClientMessage("Error: Channel name must start with '#'", fd);
    }

    channel_name = channel_name.substr(1);

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
        sendClientMessage(ERR_NOSUCHCHANNEL(clientList[fd].getNickname(), channel_name), fd);
        return;
    }

    if (!channel_it->isMember(clientList[fd].getNickname()) || !channel_it->isOperator(clientList[fd].getNickname()))
    {
        sendClientMessage(ERR_CHANOPRIVSNEEDED(clientList[fd].getNickname(), channel_name), fd);
        return;
    }

    std::map<int, IRC_Client>::iterator client_it;
    for (client_it = clientList.begin(); client_it != clientList.end(); ++client_it)
    {
        if (client_it->second.getNickname() == target_nickname)
        {
            break;
        }
    }

    if (client_it == clientList.end()) {
        sendClientMessage(ERR_NOSUCHNICK(clientList[fd].getNickname(), target_nickname), fd);
        return;
    }

    if (channel_it->getMembers().size() >= static_cast<size_t>(channel_it->getLimit())) {
        sendClientMessage("Channel " + channel_name + " is full", fd);
        return;
    }

    if (channel_it->isInvited(target_nickname)) {
        sendClientMessage(target_nickname + " is already invited to #" + channel_name, fd);
        return;
    }

    channel_it->addInvited(target_nickname);
    sendClientMessage("You invited " + target_nickname + " to #" + channel_name, fd);
    sendClientMessage(clientList[fd].getNickname() + " invited you to #" + channel_name, client_it->first);
}
