#include "IRC_Server.hpp"


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
		if (middle[i] == '\0' || middle[i] == ' ' || middle[i] == ':')
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
