#include "IRC_Server.hpp"

void IRC_Server::handle_PASS(int fd, Message message, std::string password)
{
	if (message.parameters[0] == password)
	{
		std::string welcome_message = "Welcome to the server!";
		std::string already_authenticated = "You are already authenticated!";

		for (unsigned int i=0; i < client_list.size(); ++i)
		{
			if (fd == client_list[i].GetFd())
			{
				if (client_list[i].password_checked == false)
				{
					client_list[i].password_checked = true;
					send(fd, welcome_message.c_str(), welcome_message.size(), 0);
					break;
				}
				else
				{
					send(fd, already_authenticated.c_str(), already_authenticated.size(), 0);
					break;
				}
			}
		}
	}
	else
	{
		std::string Wrong_password = "Wrong password. Please try again.";
		send(fd, Wrong_password.c_str(), Wrong_password.size(), 0);
	}
}
