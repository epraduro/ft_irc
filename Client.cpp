/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgreiner <rgreiner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:44:44 by rgreiner          #+#    #+#             */
/*   Updated: 2024/06/04 20:41:42 by rgreiner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Client.hpp"
#include"Server.hpp"

Client::Client(/* args */)
{
	addr_len = sizeof(clientAddr);
	finalbuf.resize(0);
	isOp = 0;
	bzero(buf, 1);
	isConnected = 0;
	passwordVerif = 0;
	hasNickname = 0;
	hasUsername = 0;
}

Client::~Client()
{
	
}

void	sendmsg(int clientSocket, const std::string& msg) {
	std::string newmsg = msg + "\r\n";
	send(clientSocket, newmsg.c_str(), newmsg.length(), 0);
}

std::string trim(const std::string& str) {
	std::string::const_iterator start = str.begin();
	while (start != str.end() && std::isspace(*start))
		++start;
	std::string::const_iterator end = str.end();
	end--;
	while (std::distance(start, end) > 0 && std::isspace(*end))
		--end;
	return std::string(start, end + 1);
}

std::vector<std::string> split (const std::string &s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss (s);
	std::string item;
	std::string item2;

	while (getline (ss, item, delim)) 
	{
		item2 = trim(item);
		result.push_back (item2);
	}
	return (result);
}

void	Client::verifPassword(std::vector<std::string> str, std::string password)
{
		if (passwordVerif == 1)
		{
			send(clientSocket, ":You may not reregister\n", 24, 0);
			return ;
		}
		if (str.size() != 2)
		{
			send(clientSocket, "PASS :Not enough parameters\n", 28, 0);
			return ;
		}
		if (str[1] != password)
		{
			send(clientSocket, "PASS :Invalid password\n", 23, 0);
			return ;
		}
		else
			passwordVerif = 1;
}

void	Client::newusername(std::vector<std::string> str, std::vector<std::string> tmp)
{	
	if (str.size() != 5 || tmp.empty())
	{
		send(clientSocket, "USER :Invalids arguments\n", 25, 0);
		return ;
	}
	username = str[1];
	hostname = str[2];
	servername = str[3];
	realname = str[4];
	hasUsername = 1;
	std::cout << "user : " << username << std::endl;
	std::cout << "host : " << hostname << std::endl;
	std::cout << "server : " << servername << std::endl;
	std::cout << "realname : " << realname << std::endl;
	std::cout << "nickname : " << nickname << std::endl;
	//sendmsg(clientSocket, ": USER " + username);
}

void	Client::newnickname(std::vector<std::string> str, Server server)
{
	(void)server;
	std::string	temp;

	if (str.size() != 2)
	{
		send(clientSocket, "NICK :Not enough parameters\n", 28, 0);
		return ;
	}
	if (!nickname.empty())
		temp = nickname;
	nickname = str[1];
	hasNickname = 1;
	sendmsg(clientSocket, ":" + temp + " NICK " + nickname);
}

void    joinChannel(Client &client, const std::string& channel)
{
	std::string join;

	join = ":" + client.nickname + " JOIN " + channel;
	sendmsg(client.clientSocket, join);
	//sendmsg(client.clientSocket, ": " + client.servername + " 331 " + client.nickname + " " + channel + " :no topic is set");
	// 331 no topic
	// 332 si il y a topic
}


void	Client::createChannel(std::vector<std::string> str)
{
	if (str.size() < 2)
	{
		send(clientSocket, "JOIN : Not enough parameters\n", 28, 0);
		return ;
	}
	if (server.channels.size() >= 1)
	{
		for (int i = 0; !server.channels[i].channelName.empty(); i++)
		{
			if (server.channels[i].channelName == str[1])
			{
				std::cout << "server name already exist" << std::endl;
				//if (MODE)
					joinChannel(*this, server.channels[i].channelName);
				return ;
			}
		}
	}
	server.channels.push_back(Channel(str[1]));
	for (int i = 0; !server.channels[i].channelName.empty(); i++)
	{
		if (server.channels[i].channelName == str[1])
		{
			joinChannel(*this, server.channels[i].channelName);
			std::cout << "server name : " << server.channels[i].channelName << std::endl;
		}	
	}
}

void	Client::privateMessage(std::vector<std::string> str, std::vector<std::string> tmp)
{
	std::vector<std::string> target;
	int sent = 0;
	if (str[1].find(",") != std::string::npos)
		target = split(str[1].c_str(), ',');
	else
		target.push_back(str[1]);
	if (str.size() < 3 || tmp.empty())
	{
		send(clientSocket, "PRIVMSG : Not enough parameters\n", 32, 0);
		return ;
	}
	for(unsigned long j = 0; j < target.size() ;j++)
	{
		for (unsigned long i = 0; i < server.clients.size(); i++)
		{
			if (server.clients[i].nickname == target[j])
			{
				for(unsigned long k = 0; k < inConv.size() ;k++)
				{
					if(inConv[k] == target[j])
					{
						sendmsg(server.clients[i].clientSocket, ":" + nickname + " PRIVMSG " + server.clients[i].nickname + " :" + str[2]);
						sent = 1;
					}
				}
				for(unsigned long k = 0; k < server.clients[i].inConv.size() ;k++)
				{
					if(server.clients[i].inConv[k] == target[j])
					{
						sendmsg(server.clients[i].clientSocket, ":" + nickname + " PRIVMSG " + server.clients[i].nickname + " :" + str[2]);
						sent = 1;
					}
				}
				if (sent == 0)
				{	
					sendmsg(server.clients[i].clientSocket, ":" + nickname + " PRIVMSG " + server.clients[i].nickname + " :" + str[2]);
					sendmsg(clientSocket, ":" + server.clients[i].nickname + " PRIVMSG " + nickname + " :" + str[2]);
					inConv.push_back(server.clients[i].nickname);
					server.clients[i].inConv.push_back(nickname);
				}
				break;
			}
		}
	}
}

void	Client::exec(std::vector<std::string> str, std::vector<std::string> tmp)
{
	if (hasNickname == 0)
	{
		send(clientSocket, "No nickname saved, please input a nickname by using 'NICK <newnickname>\n", 72, 0);
		return ;
	}
	if (hasUsername == 0)
	{
		send(clientSocket, "No username saved, please input a username by using 'USER <newusername>\n", 72, 0);
		return ;
	}
	if (str[0] == "JOIN")
		createChannel(str);
	if (str[0] == "PRIVMSG")
		privateMessage(str, tmp);
}

void    Client::connectClient(std::string buf, std::string password, Server server)
{
	std::vector<std::string> str;
	std::vector<std::string> tmp;
	if (buf.find(":") != std::string::npos)
	{
		tmp = split(buf, ':');
		str = split(tmp[0], ' ');
		str.push_back(tmp[1]);
	}	
	else
		str = split(buf, ' ');
	if (str.size() == 3 && str[0] == "CAP" && str[1] == "LS" && str[2] == "302")
		return ;
	if (str[0].compare("PASS") == 0)
		verifPassword(str, password);
	else if (passwordVerif == 1)
	{
		if (str[0].compare("NICK") == 0)
			newnickname(str, server);
		else if (str[0].compare("USER") == 0)
			newusername(str, tmp);
		else
			exec(str, tmp);
		if (hasNickname == 1 && hasUsername == 1)
			isConnected = 1;
	}
	else
		send(clientSocket, "You didn't input the password, you must use 'PASS <password>' to be connected to the server\n", 92, 0);
}