#include "IRC_Server.hpp"

/*
The extracted message is parsed into the components <prefix>,
   <command> and list of parameters (<params>).

	The Augmented BNF representation for this is:

	message    =  [ ":" prefix SPACE ] command [ params ] crlf
	prefix     =  servername / ( nickname [ [ "!" user ] "@" host ] )
	command    =  1*letter / 3digit
	params     =  *14( SPACE middle ) [ SPACE ":" trailing ]
			   =/ 14( SPACE middle ) [ SPACE [ ":" ] trailing ]

	nospcrlfcl =  %x01-09 / %x0B-0C / %x0E-1F / %x21-39 / %x3B-FF
					; any octet except NUL, CR, LF, " " and ":"
	middle     =  nospcrlfcl *( ":" / nospcrlfcl )
	trailing   =  *( ":" / " " / nospcrlfcl )

	SPACE      =  %x20        ; space character
	crlf       =  %x0D %x0A   ; "carriage return" "linefeed"
*/

IRC_Server::Message IRC_Server::parser_irc_server(std::string received_message)
{
	Message msg;
	std::istringstream iss(received_message);
	std::string token;

	// Parse prefix
	if (received_message[0] == ':')
	{
		std::getline(iss, token, ' ');
		msg.prefix = token.substr(1);
	}

	// Parse command
	std::getline(iss, token, ' ');
	if (isValidCommand(token)) {
		msg.command = token;
	} else {
		std::cerr << "Invalid command: " << token << std::endl;
		return msg;
	}

	// Parse parameters
	while (std::getline(iss, token, ' ')) {
		if (token[0] == ':') {
			if (isValidTrailing(token.substr(1))) {
				msg.parameters.push_back(token.substr(1));
			} else {
				std::cerr << "Invalid trailing parameter: " << token.substr(1) << std::endl;
			}
			break;
		} else if (isValidMiddle(token)) {
			msg.parameters.push_back(token);
		} else {
			std::cerr << "Invalid middle parameter: " << token << std::endl;
		}
	}

	// Parse trailing parameter
	std::string trailing;
	std::getline(iss, trailing);
	if (!trailing.empty() && isValidTrailing(trailing)) {
		msg.parameters.push_back(trailing);
	} else if (!trailing.empty()) {
		std::cerr << "Invalid trailing parameter: " << trailing << std::endl;
	}

	return msg;
}

bool IRC_Server::isValidCommand(const std::string& command) 
{
	if (command.empty()) return false;
	if (std::isalpha(command[0])) {
		for (size_t i = 1; i < command.size(); ++i) {
			if (!std::isalpha(command[i])) return false;
		}
		return true;
	} else if (std::isdigit(command[0])) {
		if (command.size() != 3) return false;
		for (size_t i = 1; i < command.size(); ++i) {
			if (!std::isdigit(command[i])) return false;
		}
		return true;
	}
	return false;
}

bool IRC_Server::isValidMiddle(const std::string& middle)
{
	for (size_t i = 0; i < middle.size(); ++i)
	{
		if (middle[i] == '\0' || middle[i] == '\r' || middle[i] == '\n' || middle[i] == ' ' || middle[i] == ':')
		{
			return false;
		}
	}
	return true;
}

bool IRC_Server::isValidTrailing(const std::string& trailing)
{
	for (size_t i = 0; i < trailing.size(); ++i)
	{
		if (trailing[i] == '\0' || trailing[i] == '\r' || trailing[i] == '\n')
		{
			return false;
		}
	}
	return true;
}







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
		std::cout << "Unknown command: " << message.command << std::endl;
	}
}


