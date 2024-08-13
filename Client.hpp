/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogregoir <ogregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:41:58 by rgreiner          #+#    #+#             */
/*   Updated: 2024/08/13 17:37:02 by ogregoir         ###   ########.fr       */
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
#include <cstring>
#include <vector>

class Channel;
class Server;

class Client
{
	protected:

	public:
		Client();
		~Client();
		int							clientSocket;
		int							isConnected;
		int							passwordVerif;
		std::vector<std::string>	inConv;
		std::string 				nickname;
		std::string					username;
		std::string 				hostname;
		std::string 				servername;
		std::string 				realname;
		int							hasNickname;
		int							hasUsername;
		sockaddr_in					clientAddr;
		socklen_t 					addr_len;
		char 						buf[1];
		std::string 				finalbuf;
		void    					connectClient(std::string buf, std::string password, Server &server);
		void						createChannel(std::vector<std::string> str, std::vector<std::string> tmp);
		void						verifPassword(std::vector<std::string> str, std::string password);
		void						newnickname(std::vector<std::string> str, Server &server);
		void						newusername(std::vector<std::string> str, std::vector<std::string> tmp);
		void						exec(Server &server, std::vector<std::string> str, std::vector<std::string> tmp);
		void						privateMessage(std::vector<std::string> str, std::vector<std::string> tmp);
		int							youre_invited(int i);
        void	                    modifynickname(std::string str);
};

std::vector<std::string> 	split (const std::string &s, char delim);
std::string 				trim(const std::string& str);
void    					sendirc(int clientSocket, const std::string& msg);

#include "Channel.hpp"