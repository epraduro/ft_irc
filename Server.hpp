/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgreiner <rgreiner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:16:27 by rgreiner          #+#    #+#             */
/*   Updated: 2024/06/19 18:33:38 by rgreiner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# pragma once
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
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
    public:
        nfds_t nfds;
        std::vector<pollfd> fds;
        std::vector<Client> clients;
        std::vector<Channel> channels;
        int serverSocket;
        Server(/* args */);
        Server(char **argv);
        void    topic_chan(std::vector<std::string> tmp, Client &client, int arg, std::vector<std::string> buffer);
        void    invite_chan(Client &client, int arg, std::vector<std::string> buffer);
        void    kick_chan(int arg, Client &client, std::vector<std::string> tmp, std::vector<std::string> buffer);
        void    update_topic(std::vector<std::string> buffer, unsigned long i);
        void    part_chan(Client &client, int arg, std::vector<std::string> buffer);
     //   void    quit_Server(Client &client, std::vector<std::string> str, std::vector<std::string> tmp);
     void   quit_Server(std::string clientName, int clientSocketcpy);
     //void       quit_Server(std::string clientName, int clientSocketcpy, std::vector<std::string> str, std::vector<std::string> tmp);
        std::string     pass;
        ~Server();
        void routine();
};

extern Server server;
void    parse_argv(Server& server, char **argv);
