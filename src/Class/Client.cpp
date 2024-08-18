/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:40:40 by aranger           #+#    #+#             */
/*   Updated: 2024/08/18 15:18:42 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
    
}

Client::Client(int socket) : _socket(socket), _auth(0), _pass(0), _nick(0)
{
}

Client::Client(const Client& src)
{
	*this = src;
}
Client& Client::operator=(const Client& src)
{
    _socket = src._socket;
    _nickname = src._nickname;
    _second_choice = src._second_choice;
    _third_choice = src._third_choice;
    _channels_in = src._channels_in;
	return (*this);
}

Client::~Client()
{
}

int		Client::getSocket() const
{
    return this->_socket;
}

bool 	Client::getAuth() const
{
    return this->_auth;
}
bool	Client::getPass() const
{
    return this->_pass;
}
bool	Client::getNick() const
{
    return this->_nick;
}

std::string Client::getUsername() const
{
	return (_username);
}


Client& Client::operator=(const Client& src)
{
	_socket = src._socket;
	_nickname = src._nickname;
	_username = src._username;
	_second_choice = src._second_choice;
	_third_choice = src._third_choice;
	_channels_in = src._channels_in;
	return (*this);
}
