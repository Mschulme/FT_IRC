#include "IRC_Server.hpp"
#include "IRC_Connected_Client.hpp"

bool signal_server_shutdown;

void IRC_Server::signal_handler_shutdown(int signum)
{
	(void) signum;
	signal_server_shutdown = true;
}


void	IRC_Server::CloseFds()
{
	for (size_t i = 0; i < client_list.size(); ++i)
	{
		close(client_list[i].GetFd());
	}
	if (socket_fd != -1)
		close(socket_fd);
}

void IRC_Server::CompressClientList(int fd)
{
	for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == fd)
		{
			fds.erase(fds.begin() + i); 
			break;
		}
	}
	for(size_t i = 0; i < client_list.size(); ++i)
	{
		if (client_list[i].GetFd() == fd)
		{
			client_list.erase(client_list.begin() + i); 
			break;
		}
	}
}


std::string IRC_Server::ReceiveNewData(int fd)
{
	char buffer[MAXMSG];
	std::memset(buffer, 0, MAXMSG);
	int n = 0;
	std::string complete_message;
	std::string line;

	while((n = recv(fd, buffer, MAXMSG - 1, 0)) > 0)
	{
		complete_message += buffer;
		std::memset(buffer, 0, MAXMSG);
		size_t pos;
		while ((pos = complete_message.find("\\r\\n")) != std::string::npos)
		{
			line = complete_message.substr(0, pos);
			complete_message.erase(0, pos + 2);
		}
	}

	if (n < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			return complete_message;
		}
		else
		{
			std::cout << "Error: recv() failed with errno " << errno << std::endl;
			CompressClientList(fd);
			close(fd);
		}
	}
	else if (n == 0)
	{
		CompressClientList(fd);
		close(fd);
	}

	return complete_message;
}


void IRC_Server::AcceptNewClient(void)
{
	IRC_Connected_Client client;
	struct sockaddr_in client_address;
	struct pollfd NewPoll;
	socklen_t len = sizeof(client_address);

	int fd_client = accept(socket_fd, (sockaddr *)&(client_address), &len);
	if (fd_client == -1)
		std::cout << "Error: accept()." << std::endl;

	if (fcntl(fd_client, F_SETFL,O_NONBLOCK) == -1)
		std::cout << "Error: fcntl()." << std::endl;

	NewPoll.fd = fd_client;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;

	client.password_checked = false;
	client.SetFd(fd_client);
	client.setIpAdd(inet_ntoa((client_address.sin_addr)));
	client_list.push_back(client);
	fds.push_back(NewPoll);
}

int IRC_Server::irc_server(int port_number, std::string password)
{
	std::string received_message = "";

	signal(SIGINT, signal_handler_shutdown);
	signal(SIGQUIT, signal_handler_shutdown);

	if (setup(port_number, password) == EXIT_SUCCESS)
		std::cout << "Log: Setup successful." << std::endl;

	while (signal_server_shutdown == false)
	{
		if((poll(&fds[0], fds.size(), -1) == -1) && signal_server_shutdown == false)
			throw(std::runtime_error("Error: poll()."));
		
		for (size_t i = 0; i < fds.size(); ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == socket_fd)
				{
					AcceptNewClient();
				}
				else
				{
					received_message = ReceiveNewData(fds[i].fd);
					IRC_Server::EventHandler(fds[i].fd, received_message, password);
				}
			}
		}
	}
	IRC_Server::CloseFds();

	return (EXIT_SUCCESS);
}