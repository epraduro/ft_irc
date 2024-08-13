

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
		std::cout << "ENTRY PASSWORD" << std::endl;
		exit(1);
	}
}

void	Server::quit_Server(std::string clientName, int clientSocketcpy)
{
	for(unsigned long i = 0; i < clients.size() ;i++)
	{
		if (clients[i].nickname != clientName)
			sendirc(clients[i].clientSocket, ":" + clientName + " QUIT " + clientName);
	}
	for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end();)
	{
		if (it->fd == clientSocketcpy) 
			{
				close(it->fd);
				nfds--;
				it = fds.erase(it);
				break;
			}
			else 
				++it;
	}
	for (unsigned int i = 0; i < channels.size(); i++)
	{
		for (std::vector<Client>::iterator it = channels[i].users.begin(); it != channels[i].users.end();)
		{
			if (it->nickname == clientName)
				it = channels[i].users.erase(it);
			else 
				++it;
		}
		if (!channels[i].op.empty())
			channels[i].op.erase(std::find(channels[i].op.begin(), channels[i].op.end(), clientName));
		if (!channels[i].invited.empty())
			channels[i].invited.erase(std::find(channels[i].invited.begin(), channels[i].invited.end(), clientName));
	}
	for (unsigned int i = 0; i < clients.size(); i++)
	{
		if (!clients[i].inConv.empty())
			clients[i].inConv.erase(std::find(clients[i].inConv.begin(), clients[i].inConv.end(), clientName));
	}
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end();)
	{
		if (it->nickname == clientName){
			it = clients.erase(it);
			break;
		}
		else 
			++it;
	}
}

Server::Server(char **argv)
{
	pass = argv[2];
	parse_argv(*this, argv);
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		std::cout << "coucou5" << std::endl; //a changer
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
	Channel channel;
	
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
				nfds++;
				fds[0].revents = 0;
			}
		if (pollret == -1)
			std::cout << "Poll error" << std::endl;
		if (nfds >= 1)
		{
			for (nfds_t i = 0; i < nfds; i++)
			{
				if (recv(clients[i].clientSocket, clients[i].buf, 1, MSG_DONTWAIT) == 1)
				{
					clients[i].finalbuf.append(clients[i].buf);
					if (clients[i].buf[0] == '\n')
					{
						if (!clients[i].finalbuf.compare(0, 4, "QUIT"))
						{
							server.quit_Server(clients[i].nickname, clients[i].clientSocket);
							break;
						}
						clients[i].connectClient(clients[i].finalbuf.data(), pass, server);
						clients[i].finalbuf.clear();
					}
					bzero(clients[i].buf, 1);
				}
					
			}
		}
	}
}

void controles(int sig)
{
	std::cout << std::endl;
	(void)sig;
	int i = server.clients.size();
	while (i != 0)
	{
		if (close(server.clients[i].clientSocket) == - 1)
			std::cerr << "Error closing server socket: " << server.clients[i].clientSocket << std::endl;
		i--;
	}
	if (close(server.serverSocket) == -1)
		std::cerr << "Error closing server socket: " << server.serverSocket << std::endl;
	std::cout << "Server is closing" << std::endl;
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