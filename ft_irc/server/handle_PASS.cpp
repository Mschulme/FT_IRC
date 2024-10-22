#include "IRC_Server.hpp"


void IRC_Server::handle_PASS(int fd, const Message& message, const std::string& password)
{
    if (client_list.find(fd) == client_list.end())
    {
        std::string error_message = "Client not found.";
        send(fd, error_message.c_str(), error_message.size(), 0);
        return;
    }

    if (message.parameters[0] == password)
    {
        IRC_Connected_Client& client = client_list[fd];
        std::string response_message;

        if (!client.password_checked)
        {
            client.password_checked = true;
            response_message = "Welcome to the server!";
        }
        else
        {
            response_message = "You are already authenticated!";
        }
        send(fd, response_message.c_str(), response_message.size(), 0);
    }
    else
    {
        std::string wrong_password = "Wrong password. Please try again.";
        send(fd, wrong_password.c_str(), wrong_password.size(), 0);
    }
}
