/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_mode.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogregoir <ogregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:08:28 by epraduro          #+#    #+#             */
/*   Updated: 2024/08/13 17:24:20 by ogregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

void Channel::key_channel(std::string key, Client &client, std::string commande) {
    (void) client;
    if (mode_act == 1 && !key.empty()) {
        password_channel = key;
        this->modes += 'k';
    }
        
    else if (mode_act == 1 && key.empty()) {
        sendirc(client.clientSocket, ":" + client.servername + " 461 " + client.nickname + commande + ERR_NEEDMOREPARAMS);
        return ;
    }
    else {
        password_channel.erase();
        this->modes.erase(std::remove(this->modes.begin(), this->modes.end(), 'k'), this->modes.end());
    }   
}

void Channel::limit_channel(std::string limit, Client &client, std::string commande) {
    unsigned long nb = strtoul(limit.c_str(), 0, 0);
    
    if (mode_act == 1 && limit.empty()) {
        sendirc(client.clientSocket, ":" + client.servername + " 461 " + client.nickname + " " + commande + " " + ERR_NEEDMOREPARAMS);
        return ;
    }
    if (mode_act == 1) {
        if (users.size() <= nb) {
            limit_user = nb;
            this->modes += 'l';
        }  
        return ;
    }
    else {
        if (limit_user) {
            limit_user = 0;
            this->modes.erase(std::remove(this->modes.begin(), this->modes.end(), 'l'), this->modes.end());
        }   
        return ;
    }
}

void Channel::invite_only() {
    if (mode_act == 1) {
        invite = 1;
        this->modes += 'i';
    }   
    else {
        invite = 0;
        this->modes.erase(std::remove(this->modes.begin(), this->modes.end(), 'i'), this->modes.end());
    }   
}

int Client::youre_invited(int i) {
    unsigned long j = 0;
	int l = 0;
    while (j != server.channels[i].invited.size()) {
        if (server.channels[i].invited[j] == nickname)
            l = 1;
        j++;
    }
    if (!l)
        return (-1);
    return (0);
}

int Channel::youre_op(int i, std::string nickname, Client &client) {
    (void) client;
    unsigned long j = 0;
    int op = 0;
    while (j <= server.channels[i].op.size()) {
        if (server.channels[i].op[j] == nickname)
            op = 1;
        j++;
    }
    if (!op) {
        return (-1);	
    }
    return (0);
}

void Channel::op_privilege(std::string nickname, Client &client, std::string commande) {
    (void) client;
    if (mode_act == 1 && !nickname.empty()) {
		op.push_back(nickname);
		operators++;
        this->modes += "o";
	}
    else if (mode_act == 1 && nickname.empty()) {
        sendirc(client.clientSocket, ":" + client.servername + " 461 " + client.nickname + " " + commande + " " + ERR_NEEDMOREPARAMS);
        return ;
    }
    else {
        std::vector<std::string>::iterator it = std::find(op.begin(), op.end(), nickname);
        if (it != op.end()) {
            op.erase(it);
            operators--;
            this->modes.erase(std::remove(this->modes.begin(), this->modes.end(), 'o'), this->modes.end());
        }
    }
}

void Channel::topic_op_chann() {
    if (mode_act == 1) {
        topic = 1;
        this->modes += "t";
    }
    else {
        topic = 0;
        this->modes.erase(std::remove(this->modes.begin(), this->modes.end(), 't'), this->modes.end());
    }
}

void Channel::parse_mode_arg(std::string str, std::string arg, Server &server, Client &client, std::string commande) {
    mode_act = 0;
    (void) server;
    if ((str[0] == '+' || str[0] == '-') && str.size() >= 2) {
        for (int i = 1; str[i]; i++) {
            if (str[0] == '+')
                mode_act = 1;
            switch(str[i]) {
                case 'i':
                    invite_only();
                    break;
                case 't':
                    topic_op_chann();
                    break;
                case 'k':
                    key_channel(arg, client, commande);
                    break;
                case 'l':
                    limit_channel(arg, client, commande);
                    break;
                case 'o':
                    op_privilege(arg, client, commande);
                    break;
                default:
                    sendirc(client.clientSocket, ":" + client.servername + " 501 " + client.nickname + " :Unknown MODE flag");
                    break;
            }
        }
    }
    else {
        sendirc(client.clientSocket, ":" + client.servername + " 501 " + client.nickname + " :Unknown MODE flag");
        return ;
    }
}


/*
 *str.size() == 4 ? str[3] : "" --> si str.size() == 4 tu lui passes str[3] sinon tu lui passes ""
*/
void Channel::setMode(std::vector<std::string> str, Server &server, std::string nickname, Client &client) {
    if (server.channels.size() >= 1) {
		for (int i = 0; !server.channels[i].channelName.empty(); i++) {
			if (server.channels[i].channelName == str[1]) {
                if (str.size() <= 2 || str[2].empty() ) {
					sendirc(client.clientSocket, ":" + client.servername + " 324 " + client.nickname + " " + server.channels[i].channelName + " " + modes);
                    return ;
                }
                if (youre_op(i, nickname, client) == -1) {
                    sendirc(client.clientSocket, ":" + client.servername + " 482" + ERR_CHANOPRIVSNEEDED);
                    return ;
                }
                parse_mode_arg(str[2], str.size() == 4 ? str[3] : "", server, client, str[0]);
			    sendirc(client.clientSocket, ": 324 " + str[2] + " " + server.channels[i].channelName + " " + modes);
                return ;
            }
		}
	}
}