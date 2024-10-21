#include "IRC_Server.hpp"


void IRC_Server::EventHandler(int fd, std::string received_message, std::string password)
{
	IRC_Server::Message message;

	message = IRC_Server::parser_irc_server(received_message);

	if (message.command == "PASS")
	{
		IRC_Server::handle_PASS(fd, message, password);
	}
	else if (message.command == "JOIN")
	{
		IRC_Server::handle_JOIN(fd, message);
	}
	else if (message.command == "NICK")
	{
		IRC_Server::handle_NICK(fd, message);
	}
	else if (message.command == "USER")
	{
		IRC_Server::handle_USER(fd, message);
	}
	else if (message.command == "PRIVMSG")
	{
		IRC_Server::handle_PRIVMSG(fd, message);
	}	
	else
	{
        std::string error_message = "Unknown command: " + message.command;
        send(fd, error_message.c_str(), error_message.size(), 0);
	}
	
}
