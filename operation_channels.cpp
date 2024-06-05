/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_channels.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:22:15 by ogregoir          #+#    #+#             */
/*   Updated: 2024/06/05 13:53:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"


void    Server::kick_chan(int arg, int clientsocket, int op, std::vector<std::string> buffer)
{
    int i = 0;
    int count = 0;
    

    if (arg < 2 || arg > 3)
        return ;
    if (op != 1)
        return ;
    while (!server.channels[count].channelName.empty())
        count++;
    while (!server.channels[i].channelName.empty())
    {
        if (server.channels[i].channelName.compare(buffer[1]))
            break;
        i++;
    }
    if (i == count && (!server.channels[count].channelName.compare(buffer[1])))
        return ;
    i = 0;
    count = 0;
    while(!server.clients[count].username.empty())
        count++;
    while(!server.clients[i].username.empty())
    {
        if (server.clients[i].username.compare(buffer[2]))
            break ;
        i++;
    }
    if (i == count && (!server.clients[count].username.compare(buffer[2])))
        return ;
    if (buffer[3] && buffer[3][0] == ":")
        sendmsg(clientSocket, ":" + username + " KICK " + buffer[1] + " " + buffer[2] + " " + buffer[3]);
    else if (buffer[3].empty())
        sendmsg(clientSocket, ":" + username + " KICK " + buffer[1] + " " + buffer[2]);
 //   std::cout << ":" + username + " KICK " + buffer[1] + " " + buffer[2] + " " + buffer[3] << std::endl;
 //   std::cout << ":" + username + " KICK " + buffer[1] + " " + buffer[2] << std::endl;
}

void    Server::invite_chan(int op, int arg, std::vector<std::string> buffer)
{
    int i = 0;
    int count = 0;

    if (arg != 3)
        return ;
    if (op != 1)
        return ;
    while (!server.clients[count].username.empty())
        count++;
    while (!server.clients[i].username.empty())
    {
        if (server.clients[i].username.compare(buffer[1]))
            break ;
        i++;
    }
    if (i == count && (!server.clients[i].username.compare(buffer[1])))
        return ;
    sendmsg(clientSocket, ":" + username + " INVITE " + buffer[1] + " " + buffer[2]);
   // std::cout << ":" + username + " INVITE " + buffer[1] + " " + buffer[2] << std::endl;
}

void    Server::topic_chan(int op, int arg, std::vector<std::string> buffer)
{
    int i = 0;
    int count = 0;

    if (arg < 1 || arg > 2)
        return ;
    while (!server.channels[count].channelName.empty())
        count++;
    while (!server.channels[i].channelName.empty())
    {
        if (server.channels[i].channelName.compare(buffer[1]))
            break ;
        i++;
    }
    if (i == count && (!server.channels[i].channelName.compare(buffer[1])))
        return ;
    if (!buffer[2].empty() && buffer[2][0] == ":")
        sendmsg(clientSocket, ":" + username + " TOPIC " + buffer[1] + " " + buffer[2]);
    else if (buffer[2].empty())
        sendmsg(clientSocket, ":" + username + " TOPIC " + buffer[1]);
}

void    Server::chan_op(int op, std::vector<std::string> buffer)
{
    int i = 0;
    
    while(!buffer[i].empty())
        i++;
    if (buffer[0].compare("INVITE"))
        invite_chan(i, op, buffer);
    else if (buffer[0].compare("TOPIC"))
        topic_chan(op, i, buffer);
    else if (buffer[0].compare("KICK"))
        kick_chan(op, i, buffer);
   // else if (buffer[0].compare("MODE"))
}