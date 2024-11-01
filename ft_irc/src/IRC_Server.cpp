#include "IRC_Server.hpp"
#include "IRC_Client.hpp"

bool g_signal_server_shutdown;

void IRC_Server::signal_handler_shutdown(int signum)
{
	(void) signum;
	g_signal_server_shutdown = true;
}


void	IRC_Server::CloseFds()
{
	for (size_t i = 0; i < client_list.size(); ++i)
	{
		close(client_list[i].GetFd());
	}
	if (socket_fd != -1)
	{
		close(socket_fd);
	}
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

	if (client_list.find(fd) != client_list.end())
	{
		client_list.erase(fd);
	}
}


void IRC_Server::AcceptNewClient(int sock, std::vector<pollfd> &pfds)
{
	int clientFd;
	sockaddr_in addr = {};
	socklen_t size = sizeof(addr);

	clientFd = accept(sock, (sockaddr *)&addr, &size);
	if (clientFd == -1)
		throw std::runtime_error("Error : Failed to accept client on the server socket!");
	else
	{
		pollfd eachNewClient;
		eachNewClient.fd = clientFd;
		eachNewClient.events = POLLIN;
		eachNewClient.revents = 0;
		pfds.push_back(eachNewClient);

		client_list[clientFd] = IRC_Client(clientFd);
	}
}

void IRC_Server::ExistingClient(std::vector<pollfd> &pfds, int i, std::string servPass)
{
	std::string buf;
	char tempBuf[1024];
	std::string firstMsg;

	int readBytes = recv(pfds[i].fd, tempBuf, sizeof(tempBuf), 0);
	if (readBytes < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		else
			throw std::runtime_error("Error : reading data from client!");
	}
	else if (readBytes == 0 || (pfds[i].revents & POLLHUP) || (pfds[i].revents & POLLERR))
	{
		CompressClientList(pfds[i].fd);
		close(pfds[i].fd);
		return;
	}
	else
	{
		buf.append(tempBuf, readBytes);
		size_t pos = buf.find("\r\n");
		while (pos != std::string::npos)
		{
			std::string message = buf.substr(0, pos);
			parser_irc_server(message, i, pfds, servPass);
			buf.erase(0, pos + 2); // +2 to remove the "\r\n"
			pos = buf.find("\r\n");
		}
	}
}


int IRC_Server::irc_server(int port_number, std::string servPass)
{
	signal(SIGINT, signal_handler_shutdown);
	signal(SIGQUIT, signal_handler_shutdown);

	if (setup(port_number) == EXIT_SUCCESS)
		std::cout << "Log: Setup successful." << std::endl;

	while (g_signal_server_shutdown == false)
	{
		if((poll(&fds[0], fds.size(), -1) == -1) && g_signal_server_shutdown == false)
			throw(std::runtime_error("Error: poll()."));
		
		for (pollFdIterator i = fds.begin(); i != fds.end(); ++i)
		{
			if ((i->revents & POLLIN) == POLLIN)
			{
				if (i->fd == socket_fd)
				{
					AcceptNewClient(socket_fd, fds);
					break;
				}
				else
				{
					size_t index = std::distance(fds.begin(), i);

					ExistingClient(fds, index, servPass);

					break;
				}
			}
		}
	}
	IRC_Server::CloseFds();

	return (EXIT_SUCCESS);
}
