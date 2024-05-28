/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgreiner <rgreiner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:44:44 by rgreiner          #+#    #+#             */
/*   Updated: 2024/05/28 15:20:16 by rgreiner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Client.hpp"

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
    while (start != str.end() && std::isspace(*start)) {
        ++start;
    }
    std::string::const_iterator end = str.end();
    do {
        --end;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

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
			send(clientSocket, ":You may not reregister\n", 25, 0);
			return ;
		}
		if (str.size() != 2)
		{
			send(clientSocket, "PASS :Not enough parameters\n", 29, 0);
			return ;
		}
		if (str[1] != password)
		{
			send(clientSocket, "PASS :Invalid password\n", 24, 0);
			return ;
		}
		else
			passwordVerif = 1;
}

void	Client::newusername(std::vector<std::string> str)
{
	if (str.size() != 2)
	{
		send(clientSocket, "USER :Not enough parameters\n", 29, 0);
		return ;
	}
	username = str[1];
	hasUsername = 1;
}

void	Client::newnickname(std::vector<std::string> str)
{
	if (str.size() != 2)
	{
		send(clientSocket, "NICK :Not enough parameters\n", 29, 0);
		return ;
	}
	nickname = str[1];
	hasNickname = 1;
}

void	Client::exec(std::vector<std::string> str)
{
	(void)str;
	if (hasNickname == 0)
	{
		send(clientSocket, "No nickname saved, please input a nickname by using 'NICK <newnickname>\n", 73, 0);
		return ;
	}
	if (hasUsername == 0)
	{
		send(clientSocket, "No username saved, please input a username by using 'USER <newusername>\n", 73, 0);
		return ;
	}
}

void    Client::connectClient(std::string buf, std::string password)
{
	std::cout << "Recive : " << buf << std::endl;
	std::vector<std::string> str;

	str = split(buf, ' ');
	if (str[0].compare("PASS") == 0)
		verifPassword(str, password);
	else if (passwordVerif == 1)
	{
		if (str[0].compare("NICK") == 0)
			newnickname(str);
		else if (str[0].compare("USER") == 0)
			newusername(str);
		else
			exec(str);
		if (hasNickname == 1 && hasUsername == 1)
			isConnected = 1;
	}
	else
		send(clientSocket, "You didn't input a password, you must use 'PASS <password>' to be connected to the server\n", 91, 0);
	
}