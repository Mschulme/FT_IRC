#include "IRC_Server.hpp"

void IRC_Server::handle_PRIVMSG(int fd, std::vector<std::string> message)
{
    IRC_Client client = clientList[fd];
    int dest_fd;

    if (message.size() < 3 || message[1].empty())
    {
        client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "PRIVMSG"), fd);
        return;
    }

    std::string target = message[1];

    std::string msg_content;
    for (size_t i = 2; i < message.size(); ++i)
    {
        msg_content += message[i];
        if (i != message.size() - 1)
        {
            msg_content += " ";
        }
    }

    if (target[0] == '#')
    {
        std::string channelName = target.substr(1);
        for (std::vector<IRC_Channel>::iterator it = channelList.begin(); it != channelList.end(); ++it)
        {
            if (it->getName() == channelName)
            {
                std::vector<IRC_Client> channelClients = it->getMembers();
                for (std::vector<IRC_Client>::iterator clientIt = channelClients.begin(); clientIt != channelClients.end(); ++clientIt)
                {
                    if (clientIt->getFd() != fd)
                    {
                        clientIt->reply(RPL_PRIVMSG(client.getPrefix(), client.getNickname(), msg_content), clientIt->getFd());
                    }
                }
                return;
            }
        }
        client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), target), fd);
    }
    else
    {
        for (size_t i = 0; i < clientList.size(); ++i)
        {
            if (clientList[i].getNickname() == target)
            {
                dest_fd = clientList[i].getFd();
                client.reply(RPL_PRIVMSG(client.getPrefix(), clientList[fd].getNickname(), msg_content), dest_fd);
                return;
            }
        }
        client.reply(ERR_NOSUCHNICK(client.getNickname(), target), fd);
    }
}
