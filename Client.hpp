/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgreiner <rgreiner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:41:58 by rgreiner          #+#    #+#             */
/*   Updated: 2024/05/23 19:54:38 by rgreiner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "iostream"
#include "poll.h"
#include "string"
#include "sstream"
#include "cstdlib"
#include "fstream"
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include<cstring>
#include<vector>

class Client
{
protected:

public:
    Client(/* args */);
    ~Client();
    int clientSocket;
    int isConnected;
    sockaddr_in clientAddr;
    socklen_t addr_len;
    std::vector<char> buf;
	std::vector<char> finalbuf;
    void    connectClient(std::string buf);
};
