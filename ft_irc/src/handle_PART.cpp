#include "IRC_Response.hpp"
#include "IRC_Client.hpp"
#include "IRC_Server.hpp"

//this command is used to leave channel
void IRC_Server::handle_PART(int fd, std::vector<std::string> message)
{
    std::string memberNames;
    std::string channelName = message[1].substr(1);
    std::vector<IRC_Channel>::iterator channelToRemove;
    bool channelExists = false;

    if (message.size() < 2) {
        sendClientMessage(PART_USAGE, fd);
        return;
    }

    for (std::vector<IRC_Channel>::iterator channelIt = channelList.begin(); channelIt != channelList.end(); ++channelIt)
    {
        if (channelIt->getName() == channelName)
        {
            channelExists = true;
            if (channelIt->isMember(clientList[fd].getNickname()))
            {
                for (size_t i = 0; i < channelIt->getMembers().size(); ++i)
                {
                    if (channelIt->getMembers()[i].getNickname() == clientList[fd].getNickname())
                    {
                        sendClientMessage(clientList[fd].getNickname() + " has left the " + channelName, channelIt->getMembers()[i].getFd());
                    }
                    else
                    {
                        sendClientMessage(clientList[fd].getNickname() + " has left the " + channelName + "\r\n", channelIt->getMembers()[i].getFd());
                    }
                }

                channelIt->removeMember(clientList[fd].getNickname());

                memberNames.clear();
                for (size_t i = 0; i < channelIt->getMembers().size(); ++i)
                {
                    for (size_t j = 0; j < channelIt->getOperators().size(); ++j)
                    {
                        if (channelIt->getOperators()[j].getNickname() == channelIt->getMembers()[i].getNickname())
                        {
                            memberNames += "@";
                        }
                    }
                    memberNames += channelIt->getMembers()[i].getNickname() + " ";
                }

                for (size_t i = 0; i < channelIt->getMembers().size(); ++i)
                {
                    sendClientMessage(RPL_NAMREPLY(channelIt->getMembers()[i].getNickname(), channelName, memberNames), channelIt->getMembers()[i].getFd());
                    sendClientMessage(RPL_ENDOFNAMES(channelIt->getMembers()[i].getNickname(), channelName), channelIt->getMembers()[i].getFd());
                }


                if (channelIt->getMembers().empty()) {
                    std::cout << "Channel " << channelIt->getName() << " is removed from server. There is no members left." << std::endl;
                    channelToRemove = channelIt;
                    break;;
                }
            }
        }
    }
    if (sizeof(channelToRemove) < 1)
    {
        channelList.erase(channelToRemove);
        return;
    }
    if (channelExists == false)
    {
        sendClientMessage(ERR_NOSUCHCHANNEL(clientList[fd].getNickname(), channelName), fd);
    }
}
