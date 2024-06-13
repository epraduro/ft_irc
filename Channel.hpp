/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: epraduro <epraduro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 18:00:46 by rgreiner          #+#    #+#             */
/*   Updated: 2024/06/12 17:20:24 by epraduro         ###   ########.fr       */
=======
/*   By: ogregoir <ogregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 18:00:46 by rgreiner          #+#    #+#             */
/*   Updated: 2024/06/12 20:10:23 by ogregoir         ###   ########.fr       */
>>>>>>> 059709b6b85d043fbf0243bcb95b84a72b042814
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

class Client;
class Channel
{
	protected:
		
	public:
		std::string					channelName;
		std::vector<Client>			users;
		std::vector<std::string>	invite;
		int							ChannelSocket;
		std::vector<std::string>	op;
		std::string					subject;
		unsigned int limit_user;
    	std::string password_channel;
		int mode_act;
		unsigned int invite;
		Channel(std::string name, std::string n_op, Client client);
		Channel();
		~Channel();
		void    setMode(std::vector<std::string> str, Server server);
		void    parse_mode_arg(std::string str, std::string arg, Server server);
        void    key_channel(std::string key);
        void    limit_channel(std::string limit);
		void 	invite_only();
};

#include "Client.hpp"