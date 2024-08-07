/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgreiner <rgreiner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:44:44 by rgreiner          #+#    #+#             */
/*   Updated: 2024/08/07 15:57:47 by rgreiner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Client.hpp"
#include"Server.hpp"

Client::Client(/* args */)
{
	addr_len = sizeof(clientAddr);
	finalbuf.resize(0);
	bzero(buf, 1);
	isConnected = 0;
	passwordVerif = 0;
	hasNickname = 0;
	servername = "DEFAULT";
	hasUsername = 0;
}

Client::~Client()
{
	
}

void	sendirc(int clientSocket, const std::string& msg) {
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

std::vector<std::string> split2 (const std::string &s, char delim) {
	std::vector<std::string> result;
	std::size_t pos = s.find(delim);
    std::string first = s.substr(0, pos);
    std::string second = s.substr(pos + 1);
	result.push_back(trim(first));
	result.push_back(trim(second));
	return (result);
}

void	Client::verifPassword(std::vector<std::string> str, std::string password)
{
		if (passwordVerif == 1)
		{
			sendirc(clientSocket, ":" + servername + " 462 PASS" + ERR_ALREADYREGISTRED);
			return ;
		}
		if (str.size() != 2)
		{
			sendirc(clientSocket, ":" + servername + " 461 PASS" + ERR_NEEDMOREPARAMS);
			return ;
		}
		if (str[1] != password)
		{
			sendirc(clientSocket, ":" + servername + " 464 PASS" + ERR_PASSWDMISMATCH);
			return ;
		}
		else
			passwordVerif = 1;
}

void	Client::newusername(std::vector<std::string> str, std::vector<std::string> tmp)
{	
	if (str.size() != 5 || tmp.empty())
	{
		sendirc(clientSocket, ":" + servername + " 461 USER" + ERR_NEEDMOREPARAMS);
		return ;
	}
	username = str[1];
	hostname = str[2];
	servername = str[3];
	realname = str[4];
	hasUsername = 1;
}

void	Client::modifynickname(std::string str)
{
	nickname = str;
}

void	modifyvalue(std::string str, Client &user)
{
	user.modifynickname(str);
}


void	Client::newnickname(std::vector<std::string> str, Server &server)
{
	std::string	temp;

	if (str.size() != 2)
	{
		sendirc(clientSocket, ":" + servername + " 461 NICK" + ERR_NEEDMOREPARAMS);
		return ;
	}
	for (unsigned long i = 0; i < server.clients.size(); i++)
	{
		if (server.clients[i].nickname == str[1])
		{
			std::cout << "PASS : " << nickname << std::endl;
			sendirc(clientSocket, ":" + servername + " 433 NICK " + str[1] + ERR_NICKNAMEINUSE);
			return;
		}
	}
	if (!nickname.empty())
	{
		temp = nickname;
		for (unsigned long i = 0; i < server.channels.size(); i++)
		{
			for (unsigned long j = 0; j < server.channels[i].users.size(); j++)
			{
				if (server.channels[i].users[j].nickname == temp)
				{
					modifyvalue(str[1], server.channels[i].users[j]);
					for(unsigned long o = 0; o < server.channels[i].users.size(); o++)
						sendirc(server.channels[i].users[o].clientSocket, ":" + temp + " NICK " + str[1]);
				}
			}
			for (unsigned long j = 0; j < server.channels[i].op.size(); j++)
			{
				if (server.channels[i].op[j] == temp)
					server.channels[i].op[j] = str[1];
			}
		}
		for (unsigned long l = 0; l < server.clients.size(); l++)
		{
			for (unsigned long k = 0; k < server.clients[l].inConv.size(); k++)
			{
				if (server.clients[l].inConv[k] == temp)
				{
					sendirc(server.clients[l].clientSocket, ":" + temp + " NICK " + str[1]);
					server.clients[l].inConv[k] = str[1];
				}
			}
			if (server.clients[l].nickname == temp)
				modifyvalue(str[1], server.clients[l]);
		}
		sendirc(clientSocket, ":" + temp + " NICK " + str[1]);
		hasNickname = 1;
		return ;
	}
	nickname = str[1];
	hasNickname = 1;
} 

void    joinChannel(Client &client, const std::string& channel)
{
	std::string join;

	join = ":" + client.nickname + " JOIN " + channel;
	sendirc(client.clientSocket, join);
}

void	Client::createChannel(std::vector<std::string> str, std::vector<std::string> tmp)
{
	if (str.size() < 2)
	{
		sendirc(clientSocket, ":" + servername + " 461 JOIN" + ERR_NEEDMOREPARAMS);
		return ;
	}
	if (str[1][0] != '#')
	{
		send(clientSocket, "JOIN : Server name must start by a #\n", 37, 0);
		return ;
	}
	if (server.channels.size() > 1)
	{
		for (int i = 0; !server.channels[i].channelName.empty(); i++)
		{
			if (server.channels[i].channelName == str[1])
			{
				if (!server.channels[i].password_channel.empty() && str.size() >= 3 && str[2] != server.channels[i].password_channel) {
					std::cout << "prbl1" << std::endl;
					return ;
				}
				if (server.channels[i].limit_user && server.channels[i].users.size() + 1 >= server.channels[i].limit_user) {
					std::cout << "prbl2" << std::endl;
					return;
				}
				if (server.channels[i].invite && server.channels[i].invited.empty()) {
					std::cout << "ce channel est sur invitation mais personne n'a recu d'invitation" << std::endl;
					return ;
				}
				if (server.channels[i].invite && !server.channels[i].invited.empty()) {
					if (youre_invited(i) == -1)
						return ;
					continue;
				}
				server.channels[i].users.push_back(*this);
				joinChannel(*this, server.channels[i].channelName);
				server.topic_chan(tmp, *this, str.size(), str);
				return ;
			}
		}
	}
	server.channels.push_back(Channel(str[1], nickname, *this));
	for (int i = 0; !server.channels[i].channelName.empty(); i++)
	{
		if (server.channels[i].channelName == str[1])
		{
			joinChannel(*this, server.channels[i].channelName);
			server.topic_chan(tmp, *this, str.size(), str);
			std::cout << server.channels[0].users[0].username << std::endl;
		}	
	}
}



void	Client::exec(Server &server, std::vector<std::string> str, std::vector<std::string> tmp)
{
	int i = str.size();
	unsigned long j = 0;
	
	while (j < server.channels.size())
	{
		if (!str[1].empty() && server.channels[j].channelName == str[1])
			break;
		j++;
	}
	std::cout << "nbr clients > " << server.clients.size() << std::endl;
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
		createChannel(str, tmp);
	else if (str[0] == "MODE")
		server.channels[j].setMode(str, server, nickname, *this);
	else if (str[0] == "PRIVMSG")
		privateMessage(str, tmp);
    else if (str[0] == "INVITE")
        server.invite_chan(*this, i, str);
    else if (str[0] == "TOPIC")
        server.topic_chan(tmp, *this, i, str);
    else if (str[0] == "KICK")
        server.kick_chan(i, *this, tmp, str);
	else if (str[0] == "PART")
		server.part_chan(*this, i, str);
	else if (str[0] != "WHO")
		sendirc(clientSocket, ":" + servername + " 421 " + str[0] + ERR_UNKNOWNCOMMAND);
}

void    Client::connectClient(std::string buf, std::string password, Server &server)
{
	std::vector<std::string> str;
	std::vector<std::string> tmp;
	
	std::cout << "buffer = " << buf << std::endl;
	std::cout << "clientS : "<<clientSocket << std::endl;
	if (buf.find(":") != std::string::npos)
	{
		tmp = split2(buf, ':');
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
			exec(server, str, tmp);
		if (hasNickname == 1 && hasUsername == 1 && isConnected == 0)
		{
			sendirc(clientSocket, ":server 001 " + nickname + " :Welcome to IRC server, " + nickname);
			isConnected = 1;
		}
	}
	else
		send(clientSocket, "You didn't input the password, you must use 'PASS <password>' to be connected to the server\n", 92, 0);
}