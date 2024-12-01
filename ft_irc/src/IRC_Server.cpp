#include "IRC_Server.hpp"
#include "IRC_Client.hpp"
#include "IRC_Channel.hpp"

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

void IRC_Server::existingClient(std::vector<pollfd> &pfds, int i, const std::string &servPass)
{
    std::string buf;
    char tempBuf[1024] = {0};
    static std::string oldBuf;

    int readBytes = recv(pfds[i].fd, tempBuf, sizeof(tempBuf) - 1, 0);
    if (readBytes < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        else
            throw std::runtime_error("Can't read data from client");
    }
    else if (readBytes == 0 || (pfds[i].revents & POLLHUP) || (pfds[i].revents & POLLERR))
    {
        compressClientList(pfds[i].fd);
        close(pfds[i].fd);
        return;
    }
    else
    {
        tempBuf[readBytes] = '\0';
        buf = tempBuf;
        if (!oldBuf.empty())
        {
            buf = oldBuf + buf;
            oldBuf.clear();
        }
        size_t pos;
        while ((pos = buf.find("\r\n")) != std::string::npos)
        {
            std::string message = buf.substr(0, pos);
            parser(message, i, pfds, servPass);
            buf.erase(0, pos + 2);
        }
        if (!buf.empty())
        {
            oldBuf = buf;
        }
    }
}


int IRC_Server::irc_server(int port_number, std::string servPass)
{
    signal(SIGINT, signalHandlerShutdown);  // Handle Ctrl+C interrupt
    signal(SIGQUIT, signalHandlerShutdown); // Handle quit signal

    // Set up the server and check if setup is successful
    if (setup(port_number) == EXIT_SUCCESS)
        std::cout << "Log: Setup successful." << std::endl;

    // Main server loop, runs until shutdown signal is received
    while (g_signal_server_shutdown == false)
    {
        // Use poll to monitor file descriptors for events indefinitely
        if ((poll(&_fds[0], _fds.size(), -1) == -1) && g_signal_server_shutdown == false)
            throw(std::runtime_error("Error: poll().")); // Throw an error if poll fails

        // Iterate through the file descriptors to check for events
        for (pollFdIterator i = _fds.begin(); i != _fds.end(); ++i)
        {
            // Check if the current file descriptor is ready for reading
            if ((i->revents & POLLIN) == POLLIN)
            {
                // If the ready file descriptor is the server socket, accept new client
                if (i->fd == _socketFD)
                {
                    acceptNewClient(_socketFD, _fds);
                    break; // Break the loop to re-poll for new events
                }
                else // Otherwise, handle an existing client
                {
                    size_t index = std::distance(_fds.begin(), i);
                    existingClient(_fds, index, servPass);
                    break; // Break the loop to re-poll for new events
                }
            }
        }
    }
    IRC_Server::closeFds(); // Close all open file descriptors

 
    return (EXIT_SUCCESS); // Return success status
}

/*
1. Registers signal handlers for graceful shutdown on SIGINT and SIGQUIT.
2. Sets up the server using the setup function and logs if successful.
3. Enters the main server loop, which continues until a shutdown signal is received.
4. Uses poll to wait for events on file descriptors:
   a. If poll fails, it throws a runtime error.
   b. Iterates through the file descriptors to check for POLLIN events.
5. If the server socket is ready for reading, it accepts a new client connection.
6. If an existing client socket is ready for reading, it processes the client's data.
7. Closes all file descriptors when the server is shutting down.
8. Returns EXIT_SUCCESS upon successful shutdown.

This function sets up and runs an IRC server, handling new and existing client connections.
*/
