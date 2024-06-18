/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_mode.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epraduro <epraduro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:08:28 by epraduro          #+#    #+#             */
/*   Updated: 2024/06/18 19:28:37 by epraduro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

/*
    TODO:

        ERR_NEEDMOREPARAMS    : le client n'a pas fourni assez de paramètres (461 "<commande> :Not enough parameters")          
        ERR_CHANOPRIVSNEEDED  : pour toute commande qui requiert d'etre op (482 "<canal> :You're not channel operator")
        ERR_NOSUCHNICK : pseudonyme passé en paramètre à la commande n'est pas actuellement utilisé (401 "<pseudonyme> :No such nick/channel")
        ERR_NOTONCHANNEL  : Renvoyé par le serveur quand un client essaie une commande affectant un canal dont il ne fait pas partie (442 "<canal> :You're not on that channel")       
        ERR_KEYSET : Clé de canal déjà définie (467 "<canal> :Channel key already set")
        ERR_UNKNOWNMODE  : Mode inconnu (472 "<caractère> :is unknown mode char to me")               
        ERR_NOSUCHCHANNEL   :le nom de canal donné est invalide (403 "<nom de canal> :No such channel")
*/

void Channel::key_channel(std::string key, Client &client, std::string commande) {
    (void) client;
    if (mode_act == 1 && !key.empty())
        password_channel = key;
    else if (mode_act == 1 && key.empty()) {
        sendirc(client.clientSocket, ":" + client.servername + " 461 " + client.nickname + commande + ERR_NEEDMOREPARAMS);
        return ;
    }
    else
        password_channel.erase();
}

void Channel::limit_channel(std::string limit, Client &client, std::string commande) {
    unsigned long nb = strtoul(limit.c_str(), 0, 0);
    if (mode_act == 1 && !limit.empty()) {
        if (users.size() <= nb)
            limit_user = nb;
        else
            limit_user = 0;
        return ;
    }
    else if (mode_act == 1 && limit.empty()) {
        sendirc(client.clientSocket, ":" + client.servername + " 461 " + client.nickname + " " + commande + " " + ERR_NEEDMOREPARAMS);
        return ;
    }
    else {
        if (limit_user)
            limit_user = 0;
        return ;
    }
}

void Channel::invite_only() {
    if (mode_act == 1)
        invite = 1;
    else
        invite = 0;
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
		operators = 1;
	}
    else if (mode_act == 1 && nickname.empty()) {
        sendirc(client.clientSocket, ":" + client.servername + " 461 " + client.nickname + " " + commande + " " + ERR_NEEDMOREPARAMS);
        return ;
    }
    else {
        std::vector<std::string>::iterator it = std::find(op.begin(), op.end(), nickname);
        if (it != op.end()) {
			op.erase(it);
			operators = 0;
		}
    }
}

void Channel::topic_op_chann() {
    if (mode_act == 1)
        topic = 1;
    else
        topic = 0;
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

void Channel::setMode(std::vector<std::string> str, Server &server, std::string nickname, Client &client) {
    std::string modes;
    if (server.channels.size() >= 1) {
		for (int i = 0; !server.channels[i].channelName.empty(); i++) {
			if (server.channels[i].channelName == str[1]) {
                if (str.size() <= 2 || str[2].empty() ) {
					modes += "+";
                    if (mode_act) {
                    	if (limit_user)
                    		modes += "l";
						if (invite)
							modes += "i";
						if (topic)
							modes += "t";
						if (!password_channel.empty())
							modes += "k";
						if (operators)
							modes += "o";
                    }
					// si mode_act = 0 supprimer les modes qui sont conserne dans la chaine modes
					sendirc(client.clientSocket, ":" + client.servername + " 324 " + client.nickname + " " + server.channels[i].channelName + " " + modes);
                    return ;
                }
                else if (youre_op(i, nickname, client) == -1) {
                    return ;
                }
                else 
                    parse_mode_arg(str[2], str.size() == 4 ? str[3] : "", server, client, str[0]);   //str.size() == 4 ? str[3] : "" --> si str.size() == 4 tu lui passes str[3] sinon tu lui passes ""
			    return ;
            }
		}
	}
}