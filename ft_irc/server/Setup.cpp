#include "IRC_Server.hpp"

int	IRC_Server::setup(int port_number, std::string password)
{
	(void) password;
	
	const int enable = 1;
	struct pollfd NewPoll;
	struct sockaddr_in server_address;
	
	std::memset((char *)&server_address, 0, sizeof(server_address));
	server_address.sin_port = htons(port_number);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
		throw(std::runtime_error("Failed to create socket."));

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
		throw(std::runtime_error("Error: setsockopt()."));

	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("Error: fcntl()."));

	if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
		throw(std::runtime_error("Error: bind()."));
		
	if (listen(socket_fd, SOMAXCONN) == -1)
		throw(std::runtime_error("Error: listen()."));

	NewPoll.fd = socket_fd;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;
	fds.push_back(NewPoll);

	return (EXIT_SUCCESS);
}
