/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgreiner <rgreiner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:44:44 by rgreiner          #+#    #+#             */
/*   Updated: 2024/05/31 14:59:34 by rgreiner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Client.hpp"
#include"Server.hpp"

Client::Client(/* args */)
{
	addr_len = sizeof(clientAddr);
	buf.resize(1);
	finalbuf.resize(0);
	isConnected = 0;
	passwordVerif = 0;
	hasNickname = 0;
	hasUsername = 0;
}

Client::~Client()
{
	
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

void	Client::newusername(std::vector<std::string> str)
{
	if (str.size() != 5 && str.size() != 6)
	{
		send(clientSocket, "USER :Invalids arguments\n", 25, 0);
		return ;
	}
	if (str[4][0] == ':')
	{
		username = str[1];
		hostname = str[2];
		servername = str[3];
		if (str.size() == 6)
			realname = str[4].substr(1, str[4].size()) + ' ' + str[5];
		else
			realname = str[4].substr(1, str[4].size());
		hasUsername = 1;
		std::cout << "user : " << username << std::endl;
		std::cout << "host : " << hostname << std::endl;
		std::cout << "server : " << servername << std::endl;
		std::cout << "realname : " << realname << std::endl;
	}
	else
		send(clientSocket, "USER :Error, username must be prefixed with a :\n", 48, 0);
		

}

void	Client::newnickname(std::vector<std::string> str, Server server)
{
	(void)server;
	if (str.size() != 2)
	{
		send(clientSocket, "NICK :Not enough parameters\n", 28, 0);
		return ;
	}
	nickname = str[1];
	hasNickname = 1;
}

void	Client::createChannel(std::vector<std::string> str)
{
	if (str.size() < 2)
	{
		send(clientSocket, "JOIN :Not enough parameters\n", 28, 0);
		return ;	
	}
	if (server.channels.size() >= 1)
	{
		for (int i = 0; server.channels[i].channelName.empty(); i++)
		{
			if (server.channels[i].channelName == str[1])
				std::cout << "server name already exist" << std::endl;
		}
	}
	else
		server.channels.push_back(Channel(str[1]));
	std::cout << "server name : " << server.channels[0].channelName << std::endl;
	
}

void	Client::exec(std::vector<std::string> str)
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
}

void    Client::connectClient(std::string buf, std::string password, Server server)
{
	std::vector<std::string> str;
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
			newusername(str);
		else
			exec(str);
		if (hasNickname == 1 && hasUsername == 1)
			isConnected = 1;
	}
	else
		send(clientSocket, "You didn't input the password, you must use 'PASS <password>' to be connected to the server\n", 92, 0);
}