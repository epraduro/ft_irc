/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgreiner <rgreiner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:44:44 by rgreiner          #+#    #+#             */
/*   Updated: 2024/05/23 19:57:01 by rgreiner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Client.hpp"

Client::Client(/* args */)
{
    addr_len = sizeof(clientAddr);
    buf.resize(1);
    finalbuf.resize(0);
    isConnected = 0;
}

Client::~Client()
{
    
}

void    Client::connectClient(std::string buf)
{
    std::cout << "Recive : " << buf << std::endl;
}