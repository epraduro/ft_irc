/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epraduro <epraduro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 18:00:46 by rgreiner          #+#    #+#             */
/*   Updated: 2024/06/19 15:31:27 by epraduro         ###   ########.fr       */
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
#include <algorithm>
#include"Client.hpp"

# define ERR_CHANOPRIVSNEEDED " :You're not channel operator" //482
# define ERR_NEEDMOREPARAMS " :Not enough parameters"   //461
# define ERR_NOTONCHANNEL " :You're not on that channel" //442
# define ERR_NOSUCHNICK " :No such nick/channel" //401
# define ERR_USERONCHANNEL " :is already on channel" //443
# define ERR_NOSUCHCHANNEL " :No such channel" //403
# define ERR_USERNOTINCHANNEL " :They aren't on that channel" //441
# define ERR_UNKNOWNMODE " :is unknown mode char to me" //472

class Client;
class Server;
class Channel
{
	protected:
		
	public:
		std::string					channelName;
		std::vector<Client>			users;
		std::vector<std::string>	invited;
		int							ChannelSocket;
		std::vector<std::string>	op;
		std::string					subject;
		unsigned int limit_user;
    	std::string password_channel;
		int mode_act;
		unsigned int invite;
		int topic;
		unsigned int operators;
		Channel(std::string name, std::string n_op, Client client);
		Channel();
		~Channel();
		void    setMode(std::vector<std::string> str, Server &server, std::string nickname, Client &client);
		void    parse_mode_arg(std::string str, std::string arg, Server &server, Client &client, std::string commande);
        void    key_channel(std::string key, Client &client, std::string commande);
        void    limit_channel(std::string limit, Client &client, std::string commande);
		void 	invite_only();
		int     youre_op(int i, std::string , Client &clientnickname);
		void	op_privilege(std::string nickname, Client &client, std::string commande);
		void 	topic_op_chann();
};

#include "Client.hpp"