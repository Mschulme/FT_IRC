#include "IRC_Response.hpp"
#include "IRC_Client.hpp"
#include "IRC_Server.hpp"


void IRC_Server::handle_NICK(int fd, std::vector<std::string> message)
{
	IRC_Client client = client_list[fd];

    if (message.size() == 1)
    {        
        return client.reply(ERR_NONICKNAMEGIVEN(client.get_nickname()), fd);
    }
    
    std::string Nickname = message[1];

    for (size_t i = 0; i < client_list.size(); ++i)
    {
        if (i != (size_t)fd)
        {
            if (client_list[i].get_nickname() == Nickname)
            {
                 client.reply(ERR_NICKNAMEINUSE(client.get_nickname()), fd);       
            }
        }
    }

    client_list[fd].set_nickname(Nickname);
    //client.welcome_message(); 
}
