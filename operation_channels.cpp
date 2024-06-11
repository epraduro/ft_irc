/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_channels.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgreiner <rgreiner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:22:15 by ogregoir          #+#    #+#             */
/*   Updated: 2024/06/11 19:31:33 by rgreiner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"
#include "Client.hpp"


void    Server::kick_chan(int arg, Client &client, std::vector<std::string> tmp, std::vector<std::string> buffer)
{
    int i = 0;
    int count = 0;
    int cpy_i;

    if (arg < 3 || arg > 4)
    {
        std::cout << "<commande> :Not enough parameters" << std::endl;
        return ;
    }  
    if (client.op != 1)
    {
         std::cout << "<canal> :You're not channel operator" << std::endl;
        return ;
    }
    count = server.channels.size();
    while (!server.channels[i].channelName.empty())
    {
        if (server.channels[i].channelName == buffer[1])
            break;
        i++;
    }
    if (i == count)
    {
        return ;
    }
    cpy_i = i;
    i = 0;
    count = server.clients.size();
    while(!server.clients[i].username.empty())
    {
        std::cout << server.clients[i].username << std::endl;
        if (server.clients[i].username == buffer[2])
            break ;
        i++;
    }
    if (i == count)
    {
        std::cout << "<pseudonyme> :No such nick/channel" << std::endl;
        return ;
    }
    if (!tmp.empty())
        sendirc(client.clientSocket, ":" + client.username + " KICK " + buffer[1] + " " + buffer[2] + " :" + buffer[3]);
    else
        sendirc(client.clientSocket, ":" + client.username + " KICK " + buffer[1] + " " + buffer[2]);
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
    std::cout << "invite : i = " << i << std::endl;
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

    if (arg < 2 || arg > 3)
    {
        std::cout << "<commande> :Not enough parameters" << std::endl;
        return ;
    }
    count = server.channels[i].channelName.size();
    while (!server.channels[i].channelName.empty())
    {
        if (server.channels[i].channelName == buffer[1])
            break ;
        i++;
    }
    if (i == count)
    {
        return ;
    }
    if (!tmp.empty())
    {
        server.channels[i].subject = buffer[2];
        sendirc(client.clientSocket, ":" + client.username + " TOPIC " + buffer[1] + " :" + buffer[2]);
    }
    else
    {
        std::cout << "PASS" << std::endl;
        if (!channels[i].subject.empty())
            sendirc(client.clientSocket, ":" + client.servername + " 332 " + client.nickname + " " + server.channels[i].channelName + " :" + server.channels[i].subject);
        else
            sendirc(client.clientSocket, ":" + client.servername + " 331 " + client.nickname + " " + server.channels[i].channelName + " :No topic is set");
    }   
}