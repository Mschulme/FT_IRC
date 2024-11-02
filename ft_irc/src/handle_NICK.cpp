#include "IRC_Response.hpp"
#include "IRC_Client.hpp"
#include "IRC_Server.hpp"


void IRC_Server::handle_NICK(int fd, std::vector<std::string> message)
{
	IRC_Client client = clientList[fd];

    if (message.size() == 1)
    {        
        return client.reply(ERR_NONICKNAMEGIVEN(client.getNickname()), fd);
    }
    
    std::string Nickname = message[1];

    for (size_t i = 0; i < clientList.size(); ++i)
    {
        if (i != (size_t)fd)
        {
            if (clientList[i].getNickname() == Nickname)
            {
                 client.reply(ERR_NICKNAMEINUSE(client.getNickname()), fd);       
            }
        }
    }

    clientList[fd].setNickname(Nickname);
    //client.welcome_message(); 
}
