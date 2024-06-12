/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_channels.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgreiner <rgreiner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:22:15 by ogregoir          #+#    #+#             */
/*   Updated: 2024/06/12 11:38:46 by rgreiner         ###   ########.fr       */
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
        sendirc(client.clientSocket, ":" + client.servername + " 461 TOPIC" + ERR_NEEDMOREPARAMS);
        return ;
    }  
    while ((server.channels.size() - 1) != i)
    {
        if (server.channels[i].channelName == buffer[1])
            break;
        i++;
    }
    if (i == server.channels.size())
    {
        sendirc(client.clientSocket, ":" + client.servername + " 403 " + server.channels[i].channelName + ERR_NOSUCHCHANNEL);
        return ;
    }
    while ((server.channels[i].users.size() - 1) != j)
    {
        if (server.channels[i].users[j].nickname == client.nickname)
            break ;
        j++;
    }
    if (j == server.channels[i].users.size())
    {
        sendirc(client.clientSocket, ":" + client.servername + " 442 " + server.channels[i].channelName + ERR_NOTONCHANNEL);
        return ;
    }
    j = 0;
    while ((server.channels[i].op[j].size() - 1) != j)
    {
        if (server.channels[i].op[j] == client.nickname)
            break ;
        j++;
    }
    if (j == server.channels[i].op.size())
    {
        sendirc(client.clientSocket, ":" + client.servername + " 482 " + server.channels[i].channelName + ERR_CHANOPRIVSNEEDED);
        return ;
    }
    j = 0;
    while ((server.channels[i].users.size() - 1) != j)
    {
        if (server.channels[i].users[j].nickname == buffer[2])
            break ;
        j++;
    }
    if (j == server.clients.size())
    {
        sendirc(client.clientSocket, ":" + client.servername + " 441 " + client.nickname + " " + server.channels[i].channelName + ERR_USERONCHANNEL);
        return ;
    }
    if (!tmp.empty())
        sendirc(client.clientSocket, ":" + client.username + " KICK " + buffer[1] + " " + buffer[2] + " :" + buffer[3]);
    else
        sendirc(client.clientSocket, ":" + client.username + " KICK " + buffer[1] + " " + buffer[2]);
}

void    Server::invite_chan(Client &client, int arg, std::vector<std::string> buffer)
{
    unsigned long i = 0;
    unsigned long j = 0;

    if (arg != 3)
    {
        sendirc(client.clientSocket, ":" + client.servername + " 461 " + client.nickname + ERR_NEEDMOREPARAMS);
        return ;
    }
    while (i != (server.channels.size() - 1))
    {
        if (server.channels[i].channelName == buffer[2])
            break ;
        i++;
    }
    if (i == server.channels.size())
    {
        sendirc(client.clientSocket, ":" + client.servername + " 442 " + server.channels[i].channelName + ERR_NOTONCHANNEL);
        return ;
    }
    while (j != (server.channels[i].op.size() - 1))
    {
        if (server.channels[i].op[j] == client.nickname)
            break ;
        j++;
    }
    if (j == server.channels[i].op[j].size())
    {
        sendirc(client.clientSocket, ":" + client.servername + " 482 " + server.channels[i].channelName + ERR_CHANOPRIVSNEEDED);
        return ;
    }
    j = 0;
    while (j != (server.channels[i].users.size() - 1))
    {
        if (server.channels[i].users[j].nickname == buffer[1])
        {
            sendirc(client.clientSocket, ":" + client.servername + " 443 " + client.nickname + " " + server.channels[i].channelName + ERR_USERONCHANNEL);
            return ;
        }
        j++;
    }
    j = 0;
    while (j != (server.clients.size() - 1))
    {
        if (server.clients[j].nickname == buffer[1])
            break ;
        j++;
    }
    if (i == server.clients.size())
    {
        sendirc(client.clientSocket, ":" + client.servername + " 401 " + client.nickname + ERR_NOSUCHNICK);
        return ;
    }
    sendirc(server.clients[j].clientSocket, ":" + client.nickname + " INVITE " + buffer[1] + " " + buffer[2]);
    sendirc(client.clientSocket, ":" + client.servername + " 341 " + client.nickname + " " + buffer[1] + " " + server.channels[i].channelName);
    //std::cout << ":" + client.nickname  + " INVITE " + buffer[1] + " " + buffer[2] << std::endl;
}

void    Server::update_topic(std::vector<std::string> buffer, unsigned long i)
{
    unsigned long j = 0;
    (void)buffer;

    while (j != server.channels[i].users.size())
    {
        sendirc(server.channels[i].users[j].clientSocket, ":" + server.channels[i].users[j].servername + " 332 " + server.channels[i].users[j].nickname + " " + server.channels[i].channelName + " :" + server.channels[i].subject);
        j++;
    }
}

void    Server::topic_chan(std::vector<std::string> tmp, Client &client, int arg, std::vector<std::string> buffer)
{
    unsigned long i = 0;
    unsigned long j = 0;

    if (tmp.empty() && arg != 1 && arg != 2)
    {
        sendirc(client.clientSocket, ":" + client.servername + " 461 " + client.nickname + ERR_NEEDMOREPARAMS);
        return ;
    }
    while (server.channels.size() != i)
    {
        if (server.channels[i].channelName == buffer[1])
            break ;
        i++;
    }
    if (i == server.channels.size())
    {
        sendirc(client.clientSocket, ":" + client.servername + " 442 " + server.channels[i].channelName + ERR_NOTONCHANNEL);
        return ;
    }
    while (j != server.channels[i].op.size())
    {
        if (server.channels[i].op[j] == client.nickname)
            break ;
        j++;
    } 
    if (j == server.channels.size())
    {
        sendirc(client.clientSocket, ":" + client.servername + " 482 " + server.channels[i].channelName + ERR_CHANOPRIVSNEEDED);
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