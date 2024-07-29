#include "client.hpp"

void error(const std::string &msg) {
	std::cerr << msg << std::endl;
	std::exit(0);
}

int main(int argc, char *argv[]) {
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];
	if (argc < 3) {
	   std::cerr << "usage " << argv[0] << " hostname port\n";
	   std::exit(0);
	}
	portno = std::atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		std::cerr << "ERROR, no such host\n";
		std::exit(0);
	}
	std::memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	std::memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");
	std::cout << "Please enter the message: ";
	std::memset(buffer, 0, 256);
	std::cin.getline(buffer, 255);
	n = write(sockfd, buffer, std::strlen(buffer));
	std::cout << n << std::endl;
	if (n < 0) 
		 error("ERROR writing to socket");
	std::memset(buffer, 0, 256);
	n = read(sockfd,buffer,255);
	if (n < 0) 
		 error("ERROR reading from socket");
	std::cout << buffer << "\n";
	close(sockfd);
	return 0;
}
