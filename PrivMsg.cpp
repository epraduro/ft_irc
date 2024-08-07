/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgreiner <rgreiner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 13:03:28 by rgreiner          #+#    #+#             */
/*   Updated: 2024/08/06 13:16:35 by rgreiner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"


void	Client::privateMessage(std::vector<std::string> str, std::vector<std::string> tmp)
{
	std::vector<std::string> target;
	int sent = 0;
	int	foundserver = 0;
	if (str.size() == 1)
	{
		sendirc(clientSocket, ":" + servername + " 411 PRIVMSG" + ERR_NORECIPIENT);
		return ;
	}
	if (str[1].find(",") != std::string::npos)
		target = split(str[1].c_str(), ',');
	else
		target.push_back(str[1]);
	if (str.size() == 2 && !tmp.empty())
	{
		sendirc(clientSocket, ":" + servername + " 411 PRIVMSG" + ERR_NORECIPIENT);
		return ;
	}
	if (tmp.empty())
	{
		sendirc(clientSocket, ":" + servername + " 412 PRIVMSG" + ERR_NOTEXTTOSEND);
		return;
	}
	for(unsigned long j = 0; j < target.size() ;j++)
	{
		if (target[j][0] == '#')
		{
			if (server.channels.size() == 0)
			{
				sendirc(clientSocket, ":" + servername + " 401 " + nickname + " " + target[j] + " :No such nick/channel");
				return ;
			}
			for (unsigned long i = 0; i < server.channels.size(); i++)
			{
				if (server.channels[i].channelName == target[j].c_str())
					{
						foundserver = 1;
						for (unsigned long k = 0; k < server.channels[i].users.size(); k++)
						{
							if (server.channels[i].users[k].nickname == nickname)
								break;
							if (k == server.channels[i].users.size())
							{
								sendirc(clientSocket, ":" + servername + " 404 " + nickname + " " + server.channels[i].channelName + " :Cannot send to channel");
								return ;
							}
						}
						for (unsigned long k = 0; k < server.channels[i].users.size(); k++)
						{ 
							if (server.channels[i].users[k].nickname != nickname)
							{
								sendirc(server.channels[i].users[k].clientSocket, ":" + nickname + " PRIVMSG " + server.channels[i].channelName + " :" + str[2]);
								sent = 1;
							}
						}
					}
				if (sent == 0 && foundserver == 0 && i == server.channels.size() - 1)
				{
					sendirc(clientSocket, ":" + servername + " 401 " + nickname + " " + target[j] + " :No such nick/channel1");
					return ;
				}
			}
		}
		else
		{
			for (unsigned long i = 0; i < server.clients.size(); i++)
			{
				if (server.clients[i].nickname == target[j])
				{
					for(unsigned long k = 0; k < inConv.size() ;k++)
					{
						if(inConv[k] == target[j])
						{
							sendirc(server.clients[i].clientSocket, ":" + nickname + " PRIVMSG " + server.clients[i].nickname + " :" + str[2]);
							sent = 1;
						}
					}
					for(unsigned long k = 0; k < server.clients[i].inConv.size() ;k++)
					{
						if(server.clients[i].inConv[k] == target[j])
						{
							sendirc(server.clients[i].clientSocket, ":" + nickname + " PRIVMSG " + server.clients[i].nickname + " :" + str[2]);
							sent = 1;
						}
					}
					if (sent == 0 || sent == 2)
					{	
						sendirc(server.clients[i].clientSocket, ":" + nickname + " PRIVMSG " + server.clients[i].nickname + " :" + str[2]);
						inConv.push_back(server.clients[i].nickname);
						server.clients[i].inConv.push_back(nickname);
						sent = 2;
					}
					break;
				}
				if (sent == 0 && i == server.clients.size() - 1)
				{
					sendirc(clientSocket, ":" + servername + " 401 " + nickname + " " + target[j] + " :No such nick/channel");
					return ;
				}
			}
		}
	}
}