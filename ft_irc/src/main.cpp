#include "IRC_Server.hpp"
#include "IRC.hpp"

/* Helpful resources
https://modern.ircdocs.horse/#irc-concepts
https://www.ucartz.com/clients/knowledgebase/658/How-to-Install-and-Use-netcat-Command-on-Linux.html
https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9
*/

int main(int argc, char *argv[])
{
	int port_number = 0;
	std::string password;
	IRC_Server irc_server;

	if (argc != 3)
	{
		std::cout << "The correct input format is: ./ircserver <port> <password>" << std::endl;
		return (EXIT_FAILURE);
	}

	try
	{
		validateInput(argv[1], argv[2]);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	port_number = std::atoi(argv[1]);
	if (port_number < 1024 or port_number > 49151)
	{
		std::cout << "Please use a port number between 1024 and 49151." << std::endl;
		return (EXIT_FAILURE);
	}
	password = argv[2];

	try
	{
		irc_server.irc_server(port_number, password);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << "IRC Server is shutdown." << std::endl;
	return (EXIT_SUCCESS);
}
