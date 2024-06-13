/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: epraduro <epraduro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:44:44 by rgreiner          #+#    #+#             */
/*   Updated: 2024/06/12 17:39:47 by epraduro         ###   ########.fr       */
=======
/*   By: ogregoir <ogregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:44:44 by rgreiner          #+#    #+#             */
/*   Updated: 2024/06/12 14:26:53 by ogregoir         ###   ########.fr       */
>>>>>>> 059709b6b85d043fbf0243bcb95b84a72b042814
/*                                                                            */
/* ************************************************************************** */

#include"Client.hpp"
#include"Server.hpp"

Client::Client(/* args */)
{
	addr_len = sizeof(clientAddr);
	finalbuf.resize(0);
	bzero(buf, 1);
	isConnected = 0;
	passwordVerif = 0;
	hasNickname = 0;
	hasUsername = 0;
}

Client::~Client()
{
	
}

void	sendirc(int clientSocket, const std::string& msg) {
	std::string newmsg = msg + "\r\n";
	send(clientSocket, newmsg.c_str(), newmsg.length(), 0);
}

std::string trim(const std::string& str) {
	std::string::const_iterator start = str.begin();
	while (start != str.end() && std::isspace(*start))
		++start;
	std::string::const_iterator end = str.end();
	end--;
	while (std::distance(start, end) > 0 && std::isspace(*end))
		--end;
	return std::string(start, end + 1);
}

std::vector<std::string> split (const std::string &s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss (s);
	std::string item;
	std::string item2;

	while (getline (ss, item, delim)) 
	{
		item2 = trim(item);
		result.push_back (item2);
	}
	return (result);
}

std::vector<std::string> split2 (const std::string &s, char delim) {
	std::vector<std::string> result;
	std::size_t pos = s.find(delim);
    std::string first = s.substr(0, pos);
    std::string second = s.substr(pos + 1);
	result.push_back(trim(first));
	result.push_back(trim(second));
	return (result);
}

void	Client::verifPassword(std::vector<std::string> str, std::string password)
{
		if (passwordVerif == 1)
		{
			send(clientSocket, ":You may not reregister\n", 24, 0);
			return ;
		}
		if (str.size() != 2)
		{
			send(clientSocket, "PASS :Not enough parameters\n", 28, 0);
			return ;
		}
		if (str[1] != password)
		{
			send(clientSocket, "PASS :Invalid password\n", 23, 0);
			return ;
		}
		else
			passwordVerif = 1;
}

void	Client::newusername(std::vector<std::string> str, std::vector<std::string> tmp)
{	
	if (str.size() != 5 || tmp.empty())
	{
		send(clientSocket, "USER :Invalids arguments\n", 25, 0);
		return ;
	}
	username = str[1];
	hostname = str[2];
	servername = str[3];
	realname = str[4];
	hasUsername = 1;
	std::cout << "user : " << username << std::endl;
	std::cout << "host : " << hostname << std::endl;
	std::cout << "server : " << servername << std::endl;
	std::cout << "realname : " << realname << std::endl;
	std::cout << "nickname : " << nickname << std::endl;
}

void	Client::newnickname(std::vector<std::string> str, Server server)
{
	std::string	temp;

	if (str.size() != 2)
	{
		send(clientSocket, "NICK :Not enough parameters\n", 28, 0);
		return ;
	}
	for (unsigned long i = 0; i < server.clients.size(); i++)
	{
		if (server.clients[i].nickname == str[1])
		{
			send(clientSocket, "NICK :Nickname already exist\n", 29, 0);
			return;
		}
	}
	if (!nickname.empty())
	{
		temp = nickname;
		std::cout << "PASS-1" << std::endl;
		for (unsigned long i = 0; i < server.channels.size(); i++)
		{
			std::cout << "PASS0" << std::endl;
			for (unsigned long j = 0; j < server.channels[i].users.size(); j++)
			{
				if (server.channels[i].users[j].nickname == temp)
				{
					std::cout << "PASS1" << std::endl;
					server.channels[i].users[j].nickname = str[1];
				}
			}
			for (unsigned long j = 0; j < server.channels[i].op.size(); j++)
			{
				if (server.channels[i].op[j] == temp)
				{
					std::cout << "PASS2" << std::endl;
					server.channels[i].op[j] = str[1];
				}
			}
		}
		for (unsigned long l = 0; l < server.clients.size(); l++)
		{
			std::cout << "PASS-2" << std::endl;
			for (unsigned long k = 0; k < server.clients[l].inConv.size(); k++)
			{
				if (server.clients[l].inConv[k] == temp)
				{
					std::cout << "PASS3" << std::endl;
					sendirc(server.clients[l].clientSocket, ":" + temp + " NICK " + str[1]);
					server.clients[l].inConv[k] = str[1];
				}
			}
			if (server.clients[l].nickname == temp)
			{
				std::cout << "PASS4" << std::endl;
				server.clients[l].nickname = str[1];
			}
		}
	}
	nickname = str[1];
	hasNickname = 1;
	sendirc(clientSocket, ":" + temp + " NICK " + nickname);
}

void    joinChannel(Client &client, const std::string& channel)
{
	std::string join;

	join = ":" + client.nickname + " JOIN " + channel;
	sendirc(client.clientSocket, join);
}

void	Client::createChannel(std::vector<std::string> str, std::vector<std::string> tmp)
{
	if (str.size() < 2)
	{
		send(clientSocket, "JOIN : Not enough parameters\n", 28, 0);
		return ;
	}
	if (str[1][0] != '#')
	{
		send(clientSocket, "JOIN : Server name must start by a #\n", 37, 0);
		return ;
	}
	if (server.channels.size() >= 1)
	{
		for (int i = 0; !server.channels[i].channelName.empty(); i++)
		{
			if (server.channels[i].channelName == str[1])
			{
				if (!server.channels[i].password_channel.empty() && str.size() >= 3 && str[2] != server.channels[i].password_channel) {
					std::cout << "prbl1" << std::endl;
					return ;
				}
				if (server.channels[i].limit_user && server.channels[i].users.size() > server.channels[i].limit_user) {
					std::cout << "prbl2" << std::endl;
					return;
				}
				//sur invite et pas invite
					// return ;
				server.channels[i].users.push_back(*this);
				joinChannel(*this, server.channels[i].channelName);
				server.topic_chan(tmp, *this, str.size(), str);
				return ;
			}
		}
	}
	server.channels.push_back(Channel(str[1], nickname, *this));
	for (int i = 0; !server.channels[i].channelName.empty(); i++)
	{
		if (server.channels[i].channelName == str[1])
		{
			joinChannel(*this, server.channels[i].channelName);
			server.topic_chan(tmp, *this, str.size(), str);
			std::cout << "channel name : " << server.channels[i].channelName << std::endl;
			std::cout << server.channels[0].users[0].username << std::endl;
		}	
	}
}

void	Client::privateMessage(std::vector<std::string> str, std::vector<std::string> tmp)
{
	std::vector<std::string> target;
	int sent = 0;
	if (str[1].find(",") != std::string::npos)
		target = split(str[1].c_str(), ',');
	else
		target.push_back(str[1]);
	if (str.size() < 3 || tmp.empty())
	{
		send(clientSocket, "PRIVMSG : Not enough parameters\n", 32, 0);
		return ;
	}
	for(unsigned long j = 0; j < target.size() ;j++)
	{
		if (target[j][0] == '#')
		{
			if (server.channels.size() == 0)
			{
				sendirc(clientSocket, ":" + servername + " 401 " + nickname + " " + target[j] + " :No such nick/channel");
				return ;
			}
			for (unsigned long i = 0; i < server.channels.size(); i++)
			{
				if (server.channels[i].channelName == target[j].c_str())
					{
						for (unsigned long k = 0; k < server.channels[i].users.size(); k++)
						{
							if (server.channels[i].users[k].nickname == nickname)
								break;
							if (k == server.channels[i].users.size())
							{
								sendirc(clientSocket, ":" + servername + " 404 " + nickname + " " + server.channels[i].channelName + " :Cannot send to channel");
								return ;
							}
						}
						for (unsigned long k = 0; k < server.channels[i].users.size(); k++)
						{
							if (server.channels[i].users[k].nickname != nickname)
							{
								sendirc(server.channels[i].users[k].clientSocket, ":" + nickname + " PRIVMSG " + server.channels[i].channelName + " :" + str[2]);
								sent = 1;
							}
						}
					}
				if (sent == 0 && i == server.channels.size() - 1)
					{
						sendirc(clientSocket, ":" + servername + " 401 " + nickname + " " + target[j] + " :No such nick/channel");
						return ;
					}
			}
		}
		else
		{
			for (unsigned long i = 0; i < server.clients.size(); i++)
			{
				if (server.clients[i].nickname == target[j])
				{
					for(unsigned long k = 0; k < inConv.size() ;k++)
					{
						if(inConv[k] == target[j])
						{
							sendirc(server.clients[i].clientSocket, ":" + nickname + " PRIVMSG " + server.clients[i].nickname + " :" + str[2]);
							sent = 1;
						}
					}
					for(unsigned long k = 0; k < server.clients[i].inConv.size() ;k++)
					{
						if(server.clients[i].inConv[k] == target[j])
						{
							sendirc(server.clients[i].clientSocket, ":" + nickname + " PRIVMSG " + server.clients[i].nickname + " :" + str[2]);
							sent = 1;
						}
					}
					if (sent == 0 || sent == 2)
					{	
						sendirc(server.clients[i].clientSocket, ":" + nickname + " PRIVMSG " + server.clients[i].nickname + " :" + str[2]);
						inConv.push_back(server.clients[i].nickname);
						server.clients[i].inConv.push_back(nickname);
						sent = 2;
					}
					break;
				}
				if (sent == 0 && i == server.clients.size() - 1)
				{
					sendirc(clientSocket, ":" + servername + " 401 " + nickname + " " + target[j] + " :No such nick/channel");
					return ;
				}
			}
		}
	}
}

void	Client::exec(Server server, std::vector<std::string> str, std::vector<std::string> tmp)
{
	(void)server;

	int i = str.size();
<<<<<<< HEAD
	unsigned long j = 0;
	
	while (j < server.channels.size())
	{
		if (!str[1].empty() && server.channels[j].channelName == str[1])
			break;
		j++;
	}
=======
>>>>>>> 059709b6b85d043fbf0243bcb95b84a72b042814
	if (hasNickname == 0)
	{
		send(clientSocket, "No nickname saved, please input a nickname by using 'NICK <newnickname>\n", 72, 0);
		return ;
	}
	if (hasUsername == 0)
	{
		send(clientSocket, "No username saved, please input a username by using 'USER <newusername>\n", 72, 0);
		return ;
	}
	if (str[0] == "JOIN")
		createChannel(str, tmp);
	if (str[0] == "MODE")
		server.channels[j].setMode(str, server);
	if (str[0] == "PRIVMSG")
		privateMessage(str, tmp);
    if (str[0] == "INVITE")
        server.invite_chan(*this, i, str);
    else if (str[0] == "TOPIC")
        server.topic_chan(tmp, *this, i, str);
    else if (str[0] == "KICK")
        server.kick_chan(i, *this, tmp, str);
}

void    Client::connectClient(std::string buf, std::string password, Server server)
{
	std::vector<std::string> str;
	std::vector<std::string> tmp;
	
	std::cout << "buffer = " << buf << std::endl;
	if (buf.find(":") != std::string::npos)
	{
		tmp = split2(buf, ':');
		str = split(tmp[0], ' ');
		str.push_back(tmp[1]);
	}
	else
		str = split(buf, ' ');
	if (str.size() == 3 && str[0] == "CAP" && str[1] == "LS" && str[2] == "302")
		return ;
	if (str[0].compare("PASS") == 0)
		verifPassword(str, password);
	else if (passwordVerif == 1)
	{
		if (str[0].compare("NICK") == 0)
			newnickname(str, server);
		else if (str[0].compare("USER") == 0)
			newusername(str, tmp);
		else
			exec(server, str, tmp);
		if (hasNickname == 1 && hasUsername == 1)
			isConnected = 1;
	}
	else
		send(clientSocket, "You didn't input the password, you must use 'PASS <password>' to be connected to the server\n", 92, 0);
}