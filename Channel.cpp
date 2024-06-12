/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epraduro <epraduro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 18:22:51 by rgreiner          #+#    #+#             */
/*   Updated: 2024/06/12 17:20:17 by epraduro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{

}

Channel::~Channel()
{
	
}

Channel::Channel(std::string name, std::string n_op, Client client)
{
    users.push_back(client);
    ChannelSocket = socket(AF_INET, SOCK_STREAM, 0);
    channelName = name;
    op.push_back(n_op);
    limit_user = 0;
    mode_act = 0;
    invite = 0;
}
