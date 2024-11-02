#include "IRC_Server.hpp"

void IRC_Server::handle_PRIVMSG(int fd, std::vector<std::string> message)
{
    IRC_Client client = clientList[fd];
    int dest_fd;

    if (message.size() < 3 || message[1].empty() || message[2].empty())
    {
        client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "PRIVMSG"), fd);
        return;
    }


    for (size_t i = 0; i < clientList.size(); ++i)
    {
        if (clientList[i].getNickname() == message[1])
            dest_fd = clientList[i].getFd();
    }
    client.reply(RPL_PRIVMSG(client.getPrefix(), clientList[fd].getNickname(), message[2]), dest_fd);
}
