/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_channels.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogregoir <ogregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:22:15 by ogregoir          #+#    #+#             */
/*   Updated: 2024/06/04 18:03:16 by ogregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"

void    Server::kick_chan(std::vector<std::string> buffer)
{
    int i = 0;
    
    if (!buffer[0].compare("KICK"))
        return ;
    while (!server.channels[i].channelName.empty())
    {
        if (server.channels[i].channelName == buffer[1])
            break;
        i++;
    }
    
}