#include "IRC_Response.hpp"
#include "IRC_Client.hpp"
#include "IRC_Server.hpp"

void IRC_Server::handle_PRIVMSG(int fd, std::vector<std::string> message)
{
    IRC_Client client = client_list[fd];
    int dest_fd;

    if (message.size() < 3 || message[1].empty() || message[2].empty())
    {
        client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "PRIVMSG"), fd);
        return;
    }


    for (size_t i = 0; i < client_list.size(); ++i)
    {
        if (client_list[i].get_nickname() == message[1])
            dest_fd = client_list[i].GetFd();
    }
    client.reply(RPL_PRIVMSG(client.get_prefix(), client_list[fd].get_nickname(), message[2]), dest_fd);
}
