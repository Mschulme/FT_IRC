#include "IRC_Server.hpp"


int IRC_Server::setup(int port_number) {
    const int enable = 1;  // Enable socket option to reuse the address.
    struct pollfd NewPoll;  // Structure to store poll information for the socket.
    struct sockaddr_in server_address;  // Structure to specify address family, IP address, and port number.
    
    std::memset((char *)&server_address, 0, sizeof(server_address));  // Initialize server_address to zero.
    server_address.sin_family = AF_INET;  // Use IPv4 address family.
    server_address.sin_addr.s_addr = INADDR_ANY;  // Accept connections on any IP address.
    server_address.sin_port = htons(port_number);  // Convert port number to network byte order.

    _socketFD = socket(AF_INET, SOCK_STREAM, 0);  // Create a socket with IPv4, stream type, and default protocol.
    if (_socketFD < 0)
        throw(std::runtime_error("Failed to create socket."));

    if (setsockopt(_socketFD, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)  // Set socket option to reuse the address.
        throw(std::runtime_error("Error: setsockopt()."));

    if (fcntl(_socketFD, F_SETFL, O_NONBLOCK) == -1)  // Set socket to non-blocking mode.
        throw(std::runtime_error("Error: fcntl()."));

    if (bind(_socketFD, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)  // Bind socket to specified IP address and port number.
        throw(std::runtime_error("Error: bind()."));
        
    if (listen(_socketFD, SOMAXCONN) == -1)  // Prepare socket to listen for incoming connections with maximum queue length.
        throw(std::runtime_error("Error: listen()."));

    NewPoll.fd = _socketFD;
    NewPoll.events = POLLIN;  // Monitor the socket for input readiness.
    NewPoll.revents = 0;
    _fds.push_back(NewPoll);

    return (EXIT_SUCCESS);
}

/*
1. Creates and configures a server socket.
2. Sets socket options to allow address reuse and non-blocking mode.
3. Binds the socket to the specified port and IP address.
4. Prepares the socket to listen for incoming connections.
5. Adds the socket to the polling vector to monitor for input readiness.

The server is now ready to accept and handle client connections.
*/


/*
Reasons for Non-Blocking Server Sockets: fcntl(_socketFD, F_SETFL, O_NONBLOCK)

    Concurrent Client Handling:

        In non-blocking mode, the server can continue executing without waiting for I/O operations (like accept, recv, or send) to complete.
		This allows the server to handle multiple client connections concurrently, instead of blocking on a single operation.

    Responsive Server:

        A non-blocking server remains responsive to new connections and existing client requests, even when dealing with slow or unresponsive clients.
		It avoids being stuck waiting for a single client's data, which could affect the overall performance and responsiveness of the server.

    Efficient Resource Utilization:

        By using non-blocking sockets along with poll or select, the server can efficiently manage I/O operations using a single thread or a small number of threads.
		This is more resource-efficient compared to using one thread per client, especially when dealing with a large number of connections.
*/
