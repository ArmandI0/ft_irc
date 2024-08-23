/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboire <dboire@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:40:40 by aranger           #+#    #+#             */
/*   Updated: 2024/08/23 13:56:46 by dboire           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
    
}

Client::Client(int socket) : _socket(socket), _auth(0), _pass(0)
{
	_operator = false;
    this->_nickname = "";
    this->_username = "";
}

Client::Client(const Client& src)
{
	*this = src;
}
Client& Client::operator=(const Client& src)
{
    this->_socket = src._socket;
    this->_auth = src._auth;
    this->_pass = src._pass;
    this->_nickname = src._nickname;
    this->_second_choice = src._second_choice;
    this->_third_choice = src._third_choice;
    this->_channels_in = src._channels_in;
	this->_entry = src._entry;
	return (*this);
}

Client::~Client()
{
    
}

/*		Getters		*/
int										Client::getSocket() 	const{return this->_socket;}
bool 									Client::getAuth() 		const{return this->_auth;}
bool									Client::getPass() 		const{return this->_pass;}
std::string								Client::getNick() 		const{return this->_nickname;}
std::string 							Client::getUsername() 	const{return (_username);}
const std::map<std::string, Channel*>	Client::getChannelsIn()	{return this->_channels_in;}
std::string								&Client::getEntry() 	{return this->_entry;}

/*      Setters     */
void	Client::setNick(const std::string &str)		{this->_nickname = str;}
void    Client::setUser(const std::string & str)	{this->_username = str;}
void	Client::setSocket(int socket)				{this->_socket = socket;}
void	Client::setPass()							{this->_pass = true;}
void    Client::setAuth()							{this->_auth = true;}
void    Client::setUsername(std::string username)	{this->_username = username;}

void    Client::setEntry(const std::string & str)	{this->_entry.append(str);}
void	Client::eraseEntry()						{this->_entry = "";}
