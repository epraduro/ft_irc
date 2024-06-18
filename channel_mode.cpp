/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_mode.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epraduro <epraduro@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 14:08:28 by epraduro          #+#    #+#             */
/*   Updated: 2024/06/17 18:07:43 by epraduro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

/*
    TODO:
        - faire en sorte qu'uniquement les op [euvent utiliser la commande MODE
*/

void Channel::key_channel(std::string key) {
    if (mode_act == 1 && !key.empty()) {
        password_channel = key;
        std::cout << "the password of this channel is: " << password_channel << std::endl;
    }
    else if (mode_act == 1 && key.empty()) {
        std::cout << "This mode need a key for the channel." << std::endl;
        return ;
    }
    else {
        std::cout << "the channel key is removed" << std::endl;
        password_channel.erase();
        std::cout << "actually the password is:" << password_channel << "/" << std::endl;
    }
}

void Channel::limit_channel(std::string limit) {
    unsigned long nb = strtoul(limit.c_str(), 0, 0);
    if (mode_act == 1 && !limit.empty()) {
        if (users.size() <= nb) {
            limit_user = nb;
            std::cout << "This channel is now limited to " << limit_user << " clients only." << users.size() << "/" << channelName << std::endl;
        }
        else { 
            limit_user = 0;
            std::cout << "There are " << users.size() << "people in this channel, making it impossible to enforce this limit." << std::endl;
        }
        return ;
    }
    else if (mode_act == 1 && limit.empty()) {
        std::cout << "This mode need a number for the user limit." << std::endl;
        return ;
    }
    else {
        if (limit_user) {
            limit_user = 50;
            std::cout << "There is no longer a client limit for this channel." << std::endl;
        }
        else
            std::cout << "There was no limit for this channel." << std::endl;
        return ;
    }
}

void Channel::invite_only() {
    if (mode_act == 1) {
        std::cout << "Acces to the channel now requires an invitation" << std::endl;
        invite = 1;
    }
    else {
        std::cout << "channel sur libre acces" << std::endl;
        invite = 0;
    }
}

int Client::youre_invited(int i) {
    unsigned long j = 0;
	int l = 0;
    while (j != server.channels[i].invited.size()) {
        std::cout << server.channels[i].invited[j] << "/" << nickname << std::endl;
        if (server.channels[i].invited[j] == nickname)
            l = 1;
        j++;
    }
    if (!l) {
        std::cout << "tu n'as pas d'invitation" << std::endl;
        return (-1);	
    }
    return (0);
}

int Channel::youre_op(int i, std::string nickname) {
    unsigned long j = 0;
    int op = 0;
    while (j <= server.channels[i].op.size()) {
        if (server.channels[i].op[j] == nickname)
            op = 1;
        j++;
    }
    if (!op) {
        std::cout << "tu n'es pas un op" << std::endl;
        return (-1);	
    }
    return (0);
}

void Channel::op_privilege(std::string nickname) {
    if (mode_act == 1) {
        std::cout << nickname << " has obtained the op privileges" << std::endl;
        op.push_back(nickname);
    }
    else {
        std::vector<std::string>::iterator it = std::find(op.begin(), op.end(), nickname);
        if (it != op.end()) {
            op.erase(it);
            std::cout << nickname << "has lost the op privileges" << std::endl;
        }
    }
}

void Channel::parse_mode_arg(std::string str, std::string arg, Server &server) {
    mode_act = 0;
    (void) server;
    if ((str[0] == '+' || str[0] == '-') && str.size() >= 2) {
        for (int i = 1; str[i]; i++) {
            if (str[0] == '+')
                mode_act = 1;
            switch(str[i]) {
                case 'i': 
                    std::cout << "invite-only is detected" << std::endl;
                    invite_only();
                    break;
                case 't':
                    std::cout << "the restrictions of the TOPIC command is detected" << std::endl;
                    break;
                case 'k':
                    std::cout << "the channel key is detected" << std::endl;
                    key_channel(arg);
                    break;
                case 'l':
                    std::cout << "the user limit to channel is detected" << std::endl;
                    limit_channel(arg);
                    break;
                case 'o':
                    std::cout << "channel operator privilege is detected" << std::endl;
                    op_privilege(arg);
                    break;
                default:
                    std::cout << "the format of arguments is incorrect" << std::endl;
                    break;
            }
        }
    }
    else {
        std::cout << "the arguments for the channel modes are incorrect" << std::endl;
        return ;
    }
}

void Channel::setMode(std::vector<std::string> str, Server &server, std::string nickname) {
	if (server.channels.size() >= 1) {
		for (int i = 0; !server.channels[i].channelName.empty(); i++) {
			if (server.channels[i].channelName == str[1]) {
                if (str.size() <= 2 || str[2].empty() ) {
                    std::cout << "No channel mode" << std::endl;
                    return ;
                }
                else if (youre_op(i, nickname) == -1) {
                    return ;
                }
                else 
                    parse_mode_arg(str[2], str.size() == 4 ? str[3] : "", server);   //str.size() == 4 ? str[3] : "" --> si str.size() == 4 tu lui passes str[3] sinon tu lui passes ""
			    return ;
            }
		}
	}
}