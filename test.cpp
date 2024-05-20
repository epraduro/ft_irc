#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "iostream"
#include "poll.h"
#include "string"
#include "sstream"
#include "cstdlib"
#include "fstream"
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include<cstring>




int main(int argc, char **argv){
	(void) argc;
	int pollret;
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		std::cout << "coucou5" << std::endl;
	int clientSocket ;
	struct pollfd fds[5];
	sockaddr_in serverAddr;
	sockaddr_in clientAddr;
	nfds_t nfds = 1;
	fds[0].fd = serverSocket;
	fds[0].events =  POLLIN;
	fds[0].revents = 0;
	socklen_t addr_len = sizeof(clientAddr);
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(atoi(argv[1]));
	if (bind(serverSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) == -1)
		std::cout << "Coucou1" << std::endl;
	if (listen(serverSocket, 10) == - 1)
		 std::cout << "Coucou2" << std::endl;
	while(1)
	{
		pollret = poll(fds, nfds, 500);
		std::cout << "pollret " << pollret << std::endl;
		std::cout << "server socket"  << serverSocket << std::endl;
		if (fds[0].revents & POLLIN)
		{
			clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_len);
			std::cout << "serverv                             f" << std::endl;
			fds[1].fd = clientSocket;
			nfds++;
		}
		//if (pollret == -1)
		//	std::cout << "Coucou4" << std::endl;
		//if (pollret == clientSocket)
		//	std::cout << "projet en cours" << std::endl;
		send(clientSocket, "Coucou\n", 7, 0);
	}
	return 0;
}
