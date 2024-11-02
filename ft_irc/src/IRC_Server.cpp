#include "IRC_Server.hpp"
#include "IRC_Client.hpp"

bool g_signal_server_shutdown;

void IRC_Server::signalHandlerShutdown(int signum)
{
	(void) signum;
	g_signal_server_shutdown = true;
}

IRC_Channel	IRC_Server::createChannel(std::string &name, int fd, std::map<int, IRC_Client> &clients)
{
	IRC_Channel chan(name, clients[fd]);
	channelList.push_back(chan);
	return (chan);
}


void	IRC_Server::closeFds()
{
	for (size_t i = 0; i < clientList.size(); ++i)
	{
		close(clientList[i].getFd());
	}
	if (_socketFD != -1)
	{
		close(_socketFD);
	}
}

void IRC_Server::compressClientList(int fd)
{
	for (size_t i = 0; i < _fds.size(); i++)
	{
		if (_fds[i].fd == fd)
		{
			_fds.erase(_fds.begin() + i); 
			break;
		}
	}

	if (clientList.find(fd) != clientList.end())
	{
		clientList.erase(fd);
	}
}


void IRC_Server::acceptNewClient(int sock, std::vector<pollfd> &pfds)
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

		clientList[clientFd] = IRC_Client(clientFd);
	}
}

void IRC_Server::existingClient(std::vector<pollfd> &pfds, int i, std::string servPass)
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
		compressClientList(pfds[i].fd);
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
			parser(message, i, pfds, servPass);
			buf.erase(0, pos + 2); // +2 to remove the "\r\n"
			pos = buf.find("\r\n");
		}
	}
}


int IRC_Server::irc_server(int port_number, std::string servPass)
{
	signal(SIGINT, signalHandlerShutdown);
	signal(SIGQUIT, signalHandlerShutdown);

	if (setup(port_number) == EXIT_SUCCESS)
		std::cout << "Log: Setup successful." << std::endl;

	while (g_signal_server_shutdown == false)
	{
		if((poll(&_fds[0], _fds.size(), -1) == -1) && g_signal_server_shutdown == false)
			throw(std::runtime_error("Error: poll()."));
		
		for (pollFdIterator i = _fds.begin(); i != _fds.end(); ++i)
		{
			if ((i->revents & POLLIN) == POLLIN)
			{
				if (i->fd == _socketFD)
				{
					acceptNewClient(_socketFD, _fds);
					break;
				}
				else
				{
					size_t index = std::distance(_fds.begin(), i);

					existingClient(_fds, index, servPass);

					break;
				}
			}
		}
	}
	IRC_Server::closeFds();

	return (EXIT_SUCCESS);
}
