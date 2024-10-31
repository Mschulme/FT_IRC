#include "IRC_Server.hpp"
#include "IRC_Response.hpp"
#include "IRC_Client.hpp"

void IRC_Server::handle_PRIVMSG(int fd, std::vector<std::string> message)
{
    /*
        Iterate through the std::map<int, class IRC_Connected_Client> client_list; client list. If the username matches, forward the message via
            send(fd, message.c_str(), message.size(), 0); to the receiver.

        The function depends on the parser. If the parser is correct, this should be straight forward.
    */

    IRC_Client client = client_list[fd];

    if (message.size() < 3 || message[1].empty() || message[2].empty())
    {
        
        client.reply(ERR_NEEDMOREPARAMS(client.get_nickname(), "PRIVMSG"), fd);
        return;
    }

    int dest_fd;

    for (size_t i = 0; i < client_list.size(); i++)
    {
        if (client_list[i].get_nickname() == message[1])
            dest_fd = client_list[i].GetFd();
    }
    client.reply(RPL_PRIVMSG(client.get_prefix(), client_list[fd].get_nickname(), message[2]), dest_fd);
}
