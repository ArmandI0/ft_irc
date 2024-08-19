/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:40:40 by aranger           #+#    #+#             */
/*   Updated: 2024/08/19 09:30:20 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
    
}

Client::Client(int socket) : _socket(socket), _auth(0), _pass(0)
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

std::string	Client::getNick() const
{
    return this->_nickname;
}

std::string Client::getUsername() const
{
	return (_username);
}


// Setters

void	Client::setNick(const std::string &str)
{
	this->_nickname = str;
}

void	Client::setSocket(int socket)
{
	this->_socket = socket;
}

void	Client::setPass()
{
	this->_pass = true;
}

void    Client::setAuth()
{
    this->_auth = true;
}

