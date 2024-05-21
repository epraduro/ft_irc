

#include "server.hpp"


Server::Server(char **argv)
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		std::cout << "coucou5" << std::endl;

	nfds = 0;
	pollfd tmp;
	tmp.fd = serverSocket;
	tmp.events = POLLIN;
	tmp.revents = 0;
	fds.push_back(tmp);
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(atoi(argv[1]));
	if (bind(serverSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) == -1)
		std::cout << "Coucou1" << std::endl;
	if (listen(serverSocket, 10) == - 1)
		 std::cout << "Coucou2" << std::endl;
}

Server::~Server()
{

}

Server::Server()
{

}

void	Server::routine()
{
	int pollret;

	while(1)
	{
		std::cout << "hello" << std::endl;
		pollret = poll(fds.data(),1 , 500);
		std::cout << "pollret " << pollret << std::endl;
		std::cout << "server socket"  << serverSocket << std::endl;
		if (fds[0].revents & POLLIN)
		{
			this->clients.push_back(Client());
			this->clients[nfds].clientSocket = accept(serverSocket, (struct sockaddr*)&clients[nfds].clientAddr, &clients[nfds].addr_len);
			fds[nfds].fd = clients[nfds].clientSocket;
			send(clients[0].clientSocket, "Coucou\n", 7, 0);
			nfds++;
		}
		//if (pollret == -1)
		//	std::cout << "Coucou4" << std::endl;
		//if (pollret == clientSocket)
		//	std::cout << "projet en cours" << std::endl;
	}
}

void controles(int sig)
{
	//free();
	std::cout << "cc" << std::endl;
	(void)sig;
	//close(server.serverSocket);
	exit(0);
}

int main(int argc, char **argv){
	(void)argc;
	(void)argv;
	signal(SIGINT, &controles);
	server = Server(argv);
	server.routine();
	return 0;
}
