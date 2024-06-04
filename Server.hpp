/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogregoir <ogregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:16:27 by rgreiner          #+#    #+#             */
/*   Updated: 2024/06/04 18:01:11 by ogregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# pragma once
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include "iostream"
# include "poll.h"
# include "string"
# include "sstream"
# include "cstdlib"
# include "fstream"
# include <unistd.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <cstring>
# include <cctype>
# include <vector>
# include <csignal>
# include"Channel.hpp"

class Client;
class Server
{
    private:
        
        sockaddr_in serverAddr;
        nfds_t nfds;
        std::vector<pollfd> fds;
        
    public:
        std::vector<Client> clients;
        std::vector<Channel> channels;
        int serverSocket;
        Server(/* args */);
        Server(char **argv);
        void kick_chan(std::vector<std::string> buffer);
        std::string     pass;
        ~Server();
        void routine();
};

extern Server server;
void    parse_argv(Server& server, char **argv);
