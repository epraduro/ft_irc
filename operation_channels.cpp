/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_channels.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogregoir <ogregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:22:15 by ogregoir          #+#    #+#             */
/*   Updated: 2024/06/05 19:22:32 by ogregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"
#include "Client.hpp"


void    Server::kick_chan(int arg, Client &client, std::vector<std::string> tmp, std::vector<std::string> buffer)
{
    int i = 0;
    int count = 0;

    std::cout << "arg = " << arg << std::endl;
    if (arg < 2 || arg > 3)
        return ;
    if (client.op != 1)
        return ;
    count = server.channels.size();
    while (!server.channels[i].channelName.empty())
    {
        if (server.channels[i].channelName == buffer[1])
            break;
        i++;
    }
    if (i == count)
        return ;
    i = 0;
    count = server.clients.size();
    while(!server.clients[i].username.empty())
    {
        if (server.clients[i].username == buffer[2])
            break ;
        i++;
    }
    if (i == count)
        return ;
    if (!buffer[3].empty() && !tmp.empty())
        sendirc(client.clientSocket, ":" + client.username + " KICK " + buffer[1] + " " + buffer[2] + " :" + buffer[3]);
    else if (buffer[3].empty())
        sendirc(client.clientSocket, ":" + client.username + " KICK " + buffer[1] + " " + buffer[2]);
 //   std::cout << ":" + username + " KICK " + buffer[1] + " " + buffer[2] + " " + buffer[3] << std::endl;
 //   std::cout << ":" + username + " KICK " + buffer[1] + " " + buffer[2] << std::endl;
}

void    Server::invite_chan(Client &client, int arg, std::vector<std::string> buffer)
{
    int i = 0;
    int count = 0;

    if (arg != 3)
    {
        std::cout << "<commande> :Not enough parameters" << std::endl;
        return ;
    }
    std::cout << "buffer[2] = " << buffer[2] << std::endl;
    if (client.op != 1)
    {
        std::cout << "<canal> :You're not channel operator" << std::endl;
        return ;
    }
    count = server.clients.size();
    while (!server.clients[i].nickname.empty())
    {
        if (server.clients[i].nickname == buffer[1])
            break ;
        i++;
    }
    if (i == count)
    {
        std::cout << "<pseudonyme> :No such nick/channel" << std::endl;
        return ; 
    }
    sendirc(server.clients[i].clientSocket, ":" + client.nickname + " INVITE " + buffer[1] + " " + buffer[2]);
    std::cout << ":" + client.nickname  + " INVITE " + buffer[1] + " " + buffer[2] << std::endl;
}

void    Server::topic_chan(std::vector<std::string> tmp, Client &client, int arg, std::vector<std::string> buffer)
{
    int i = 0;
    int count = 0;

    if (arg < 1 || arg > 2)
        return ;
    while (!server.channels[count].channelName.empty())
        count++;
    while (!server.channels[i].channelName.empty())
    {
        if (server.channels[i].channelName == buffer[1])
            break ;
        i++;
    }
    if (i == count)
        return ;
    if (!buffer[2].empty() && !tmp.empty())
        sendirc(client.clientSocket, ":" + client.username + " TOPIC " + buffer[1] + " :" + buffer[2]);
    else if (buffer[2].empty())
        sendirc(client.clientSocket, ":" + client.username + " TOPIC " + buffer[1]);
    
}