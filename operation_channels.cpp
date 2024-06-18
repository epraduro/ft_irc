/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_channels.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogregoir <ogregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:22:15 by ogregoir          #+#    #+#             */
/*   Updated: 2024/06/18 17:04:21 by ogregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

void    Server::kick_chan(int arg, Client &client, std::vector<std::string> tmp, std::vector<std::string> buffer)
{
	unsigned long i = 0;
	unsigned long j = 0;

	if (arg < 3 || arg > 4)
	{
		sendirc(client.clientSocket, ":" + client.servername + " 461 KICK" + ERR_NEEDMOREPARAMS);
		return ;
	}
	while (server.channels.size() != i)
	{
		if (server.channels[i].channelName == buffer[1])
			break;
		i++;
	}
	if (i == server.channels.size())
	{
		sendirc(client.clientSocket, ":" + client.servername + " 403 " + client.nickname + " " + buffer[1] + ERR_NOSUCHCHANNEL);
		return ;
	}
	while ((server.channels[i].users.size()) != j)
	{
		if (server.channels[i].users[j].nickname == client.nickname)
			break ;
		j++;
	}
	if (j == server.channels[i].users.size())
	{
		sendirc(client.clientSocket, ":" + client.servername + " 442 " + buffer[1] + ERR_NOTONCHANNEL);
		return ;
	}
	j = 0;
	while ((server.channels[i].op.size()) != j)
	{
		if (server.channels[i].op[j] == client.nickname)
			break ;
		j++;
	}
	if (j == server.channels[i].op.size())
	{
		sendirc(client.clientSocket, ":" + client.servername + " 482 " + client.nickname + " " + buffer[1] + ERR_CHANOPRIVSNEEDED);
		return ;
	}
	j = 0;
	while ((server.channels[i].users.size()) != j)
	{
		if (server.channels[i].users[j].nickname == buffer[2])
			break ;
		j++;
	}
	if (j == server.clients.size())
	{
		sendirc(client.clientSocket, ":" + client.servername + " 441 " + client.nickname + " " + server.channels[i].channelName + ERR_USERNOTINCHANNEL);
		return ;
	}
	if (!tmp.empty())
		sendirc(client.clientSocket, ":" + client.nickname + " KICK " + buffer[1] + " " + buffer[2] + " :" + buffer[3]);
	else
		sendirc(client.clientSocket, ":" + client.nickname + " KICK " + buffer[1] + " " + buffer[2]);
	j = 0;
	if (!tmp.empty())
	{
		while (j != server.channels[i].users.size())
		{
			if (client.nickname != server.channels[i].users[j].nickname)
				sendirc(server.channels[i].users[j].clientSocket, ":" + client.nickname + " KICK " + buffer[1] + " " + buffer[2] + " " + buffer[3]);
			j++;
		}
	}
	else
	{
		while (j != server.channels[i].users.size())
		{
			if (client.nickname != server.channels[i].users[j].nickname)
				sendirc(server.channels[i].users[j].clientSocket, ":" + client.nickname + " KICK " + buffer[1] + " " + buffer[2]);
			j++;
		}
	}
	std::cout << "1 = " << channels[i].users.size() << std::endl;
	for (std::vector<Client>::iterator it = server.channels[i].users.begin(); it != server.channels[i].users.end(); ) {
		if (it->nickname == buffer[2]) 
		{
			it = server.channels[i].users.erase(it);
		} 
		else 
		{
			++it;
		}
	}
	std::cout << "2 = " << channels[i].users.size() << std::endl;
	//server.channels[i].users.erase(std::remove(server.channels[i].users.begin(), server.channels[i].users.end(), buffer[2]), server.channels[i].users.end());
}

void    Server::invite_chan(Client &client, int arg, std::vector<std::string> buffer)
{
	unsigned long i = 0;
	unsigned long j = 0;

	if (arg != 3)
	{
		sendirc(client.clientSocket, ":" + client.servername + " 461 INVITE" + ERR_NEEDMOREPARAMS);
		return ;
	}
	while (i != (server.channels.size() - 1))
	{
		if (server.channels[i].channelName == buffer[2])
			break ;
		i++;
	}
	while (j != server.channels[i].users.size())
	{
		if (server.channels[i].users[j].nickname == client.nickname)
			break ;
		j++;
	}
	if (j == server.channels[i].users.size())
	{
		sendirc(client.clientSocket, ":" + client.servername + " 442 " + buffer[2] + ERR_NOTONCHANNEL);
		return ;
	}
	j = 0;
	while (j != server.channels[i].op.size())
	{
		if (server.channels[i].op[j] == client.nickname)
			break ;
		j++;
	}
	if (j == server.channels[i].op.size())
	{
		sendirc(client.clientSocket, ":" + client.servername + " 482 " + buffer[2] + ERR_CHANOPRIVSNEEDED);
		return ;
	}
	j = 0;
	while (j != server.channels[i].users.size())
	{
		if (server.channels[i].users[j].nickname == buffer[1])
		{
			sendirc(client.clientSocket, ":" + client.servername + " 443 " + client.nickname + " " + buffer[2] + ERR_USERONCHANNEL);
			return ;
		}
		j++;
	}
	j = 0;
	while (j != (server.clients.size()))
	{
		if (server.clients[j].nickname == buffer[1])
			break ;
		j++;
	}
	if (j == server.clients.size())
	{
		sendirc(client.clientSocket, ":" + client.servername + " 401 " + buffer[1] + ERR_NOSUCHNICK);
		return ;
	}
	sendirc(server.clients[j].clientSocket, ":" + client.nickname + " INVITE " + buffer[1] + " :" + buffer[2]);
	sendirc(client.clientSocket, ":" + client.servername + " 341 " + client.nickname + " " + buffer[1] + " " + buffer[2]);
	server.channels[i].invited.push_back(buffer[1]);
	j = 0;
	while (j != server.channels[i].users.size())
	{
		if (client.nickname != server.channels[i].users[j].nickname)
			sendirc(server.channels[i].users[j].clientSocket, ":" + client.nickname + " INVITE " + buffer[1] + " :" + buffer[2]);
		j++;
	}
}

void    Server::update_topic(std::vector<std::string> buffer, unsigned long i)
{
	unsigned long j = 0;
	(void)buffer;

	while (j != (server.channels[i].users.size() - 1))
	{
		sendirc(server.channels[i].users[j].clientSocket, ":" + server.channels[i].users[j].servername + " 332 " + server.channels[i].users[j].nickname + " " + server.channels[i].channelName + " :" + server.channels[i].subject);
		j++;
	}
}

void    Server::topic_chan(std::vector<std::string> tmp, Client &client, int arg, std::vector<std::string> buffer)
{
	unsigned long i = 0;
	unsigned long j = 0;

	if (tmp.empty() && arg != 2 && arg != 3)
	{
		sendirc(client.clientSocket, ":" + client.servername + " 461 TOPIC" + ERR_NEEDMOREPARAMS);
		return ;
	}
	while ((server.channels.size()) != i)
	{
		if (server.channels[i].channelName == buffer[1])
			break ;
		i++;
	}
	if (i == server.channels.size())
	{
		sendirc(client.clientSocket, ":" + client.servername + " 403 " + client.nickname + buffer[1] + ERR_NOSUCHCHANNEL);
		return ;
	}
	if (server.channels[i].topic == 1)
	{
		while (j != (server.channels[i].op.size()))
		{
			if (server.channels[i].op[j] == client.nickname)
				break ;
			j++;
		}
		if (j == server.channels.size())
		{
			sendirc(client.clientSocket, ":" + client.servername + " 482 " + buffer[1] + ERR_CHANOPRIVSNEEDED);
			return ;
		}
	}
	j = 0;
	while (j != (server.channels[i].users.size()))
	{
		if (server.channels[i].users[j].nickname == client.nickname)
			break ;
		j++;
	}
	if (j == server.channels[i].users.size())
	{
		sendirc(client.clientSocket, ":" + client.servername + " 442 " + buffer[1] + ERR_NOTONCHANNEL);
		return ;
	}
	j = 0;
	while (j != (server.channels[i].op.size()))
	{
		if (server.channels[i].op[j] == client.nickname)
			break ;
		j++;
	} 
	if (j == server.channels.size())
	{
		sendirc(client.clientSocket, ":" + client.servername + " 482 " + buffer[1] + ERR_CHANOPRIVSNEEDED);
		return ;
	}
	if (!tmp.empty())
	{
		server.channels[i].subject = buffer[2];
		sendirc(client.clientSocket, ":" + client.username + " TOPIC " + buffer[1] + " :" + buffer[2]);
		update_topic(buffer, i);
	}
	else
	{
		if (!channels[i].subject.empty())
			sendirc(client.clientSocket, ":" + client.servername + " 332 " + client.nickname + " " + server.channels[i].channelName + " :" + server.channels[i].subject);
		else
			sendirc(client.clientSocket, ":" + client.servername + " 331 " + client.nickname + " " + server.channels[i].channelName + " :No topic is set");
	}
}