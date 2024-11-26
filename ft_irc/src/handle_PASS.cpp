#include "IRC_Server.hpp"
#include "IRC_Client.hpp"
#include "IRC_Response.hpp"

void IRC_Server::handle_PASS(std::vector<std::string>& message, int fd, const std::string& serverPass)
{
    if (message.size() < 2)
    {
        clientList[fd].reply(ERR_NEEDMOREPARAMS(clientList[fd].getNickname(), "PASS"), fd);
        return;
    }

    if (clientList[fd].getAuthStatus() == true)
    {
        clientList[fd].reply(ERR_ALREADYREGISTERED(clientList[fd].getNickname()), fd);
        return;
    }

    std::string inputPass = message[1];
    if (inputPass.size() > 0 && inputPass[0] == ':')
    {
        inputPass = message[1].substr(1);
        for (std::vector<std::string>::size_type i = 2; i < message.size(); ++i)
        {
            inputPass += " " + message[i];
        }
    }

    if (serverPass != inputPass)
    {
        clientList[fd].reply(ERR_PASSWDMISMATCH(clientList[fd].getNickname()), fd);
        return;
    }

    clientList[fd].setAuthStatus(true);
}
