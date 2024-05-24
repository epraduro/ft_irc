/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:16:27 by rgreiner          #+#    #+#             */
/*   Updated: 2024/05/24 13:41:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP
#endif
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
# include "Client.hpp"
# include <csignal>

class Server
{
    private:
        
        sockaddr_in serverAddr;
        nfds_t nfds;
        std::vector<pollfd> fds;
        std::vector<Client> clients;
        
    public:
        int serverSocket;
        Server(/* args */);
        Server(char **argv);
        std::string     pass;
        ~Server();

        void routine();
};

Server server;
void    parse_argv(Server& server, char **argv);