/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogregoir <ogregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 18:00:46 by rgreiner          #+#    #+#             */
/*   Updated: 2024/06/11 20:21:38 by ogregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <poll.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>
#include <vector>
#include"Client.hpp"

# define ERR_CHANOPRIVSNEEDED " :You're not channel operator" //482
# define ERR_NEEDMOREPARAMS " :Not enough parameters"   //461
# define ERR_NOTONCHANNEL " :You're not on that channel" //442
# define ERR_NOSUCHNICK " :No such nick/channel" //401
# define ERR_USERONCHANNEL " :is already on channel" //443
# define ERR_NOSUCHCHANNEL " :No such channel" //403
# define ERR_USERNOTINCHANNEL " :They aren't on that channel" //441

class Channel
{
    protected:
        
    public:
        std::string channelName;
        std::vector<Client> users;
        int ChannelSocket;
        std::vector<std::string> op;
        std::string subject;
        Channel(std::string name, std::string n_op, Client client);
        Channel();
        ~Channel();
};