

#include "Server.hpp"


Server::Server(char **argv)
{
	PASS = argv[2];
	std::cout << "Pas" << PASS << std::endl;
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
	{
		std::cout << "Bind failed, try a new port" << std::endl;
		exit (1);
	}
	if (listen(serverSocket, 10) == - 1)
		 std::cout << "Listen failed" << std::endl;
	std::cout << "Server is open" << std::endl;
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
	pollfd tmp;
	std::vector<char> buf(100);
	while(1)
	{
		pollret = poll(fds.data(), nfds + 1 ,500);
			if (fds[0].revents & POLLIN)
			{
				clients.push_back(Client());
				clients[nfds].clientSocket = accept(serverSocket, (struct sockaddr*)&clients[nfds].clientAddr, &clients[nfds].addr_len);
				tmp.fd = clients[nfds].clientSocket;
				tmp.events = POLL_OUT;
				tmp.revents = 0;
				fds.push_back(tmp);
				send(clients[nfds].clientSocket, "Coucou\n", 7, 0);
				if (nfds >= 1)
					send(clients[0].clientSocket, "Another client is connected\n", 29, 0);
				nfds++;
				fds[0].revents = 0;
			}
		if (pollret == -1)
			std::cout << "Poll error" << std::endl;
		if (nfds >= 1)
		{
			for (nfds_t i = 0; i <= nfds; i++)
			{
				if (recv(this->clients[i].clientSocket, buf.data(), buf.size(), MSG_DONTWAIT) > 0)
				{
					if (clients[i].isConnected == 0)
						clients[i].connectClient(buf.data());
					else
						std::cout << "Is connected and recive : " << buf.data() << std::endl;
					buf.clear();
					buf.resize(100);
				}
					
			}
		}
	}
}

void controles(int sig)
{
	//free();
	std::cout << std::endl;
	std::cout << "Server is closing" << std::endl;
	(void)sig;
	close(server.serverSocket);
	exit(0);
}

int main(int argc, char **argv){
	
	if (argc != 3)
	{
		std::cout << "Invalid arguments" << std::endl;
		return (1);
	}
	signal(SIGINT, &controles);
	server = Server(argv);
	server.routine();
	return 0;
}
