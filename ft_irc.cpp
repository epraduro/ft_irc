

#include "Server.hpp"
#include "Client.hpp"

Server server;

void    parse_argv(Server& server, char **argv)
{
	int	port;
	int i = 0;

	port = atoi(argv[1]);
	while(argv[1][i])
	{
		if (!isdigit(argv[1][i]))
		{
			std::cout << "PORT IS NOT NUMERIC" << std::endl;
			exit(1);
		}
		i++;
	}
	if (port < 0 || port > 65535)
	{
		std::cout << "PORT INVALID" << std::endl;
		exit(1);
	}
    server.pass = argv[2];
    if (server.pass.empty())
	{
		std::cout << "ENTRY PASSEWORD" << std::endl;
		exit(1);
	}
}

Server::Server(char **argv)
{
	pass = argv[2];
	//std::cout << "Password : " << PASS << std::endl;
	parse_argv(*this, argv);
	//std::cout << "PASSWORD = " << pass << std::endl;
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
	
	while(1)
	{
		pollret = poll(fds.data(), nfds + 1 , 10);
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
					send(clients[0].clientSocket, "Another client is connected\n", 28, 0);
				nfds++;
				fds[0].revents = 0;
			}
		if (pollret == -1)
			std::cout << "Poll error" << std::endl;
		if (nfds >= 1)
		{
			for (nfds_t i = 0; i < nfds; i++)
			{
				if (recv(clients[i].clientSocket, clients[i].buf.data(), clients[i].buf.size(), MSG_DONTWAIT) == 1)
				{
					clients[i].finalbuf.push_back(clients[i].buf[0]);
					if (clients[i].buf[0] == '\n')
					{
						clients[i].connectClient(clients[i].finalbuf.data(), pass, server);
						std::vector<char>().swap(clients[i].finalbuf);
					}
					clients[i].buf.clear();
					clients[i].buf.resize(1);
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
