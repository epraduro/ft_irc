/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgreiner <rgreiner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 18:00:46 by rgreiner          #+#    #+#             */
/*   Updated: 2024/06/11 19:32:00 by rgreiner         ###   ########.fr       */
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