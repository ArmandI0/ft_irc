/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboire <dboire@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 21:07:07 by dboire            #+#    #+#             */
/*   Updated: 2024/08/23 13:54:58 by dboire           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string& name, Client* creator, Server* serv): _name(name), _server(serv), _key(""), _limit_user(-1), _invite_only(false), _topic_protection(false), _channel_topic("")
{
	addClientToCh(creator);
}

Channel::Channel()
{
}

Channel::Channel(const Channel& src)
{
	*this = src;
}

Channel::~Channel()
{
}

void	Channel::addClientToCh(Client* client)
{
	_clients.insert(std::make_pair(client->getNick(), client));
	printUsersInChannel(client, this->_name);
}

void	Channel::delClient(std::string nickname)
{
	(void)nickname;
}

void	Channel::delChannel()
{
	_server->delChannel(this->_channel_topic);
}

Channel& Channel::operator=(const Channel& src)
{
	_channel_topic = src._channel_topic;
	_clients = src._clients;
	_operators = src._operators;
	_server = src._server;
	_key = src._key;
	_limit_user = src._limit_user;
	_topic_protection = src._topic_protection;
	_invite_only = src._invite_only;
	return (*this);
}

void	Channel::removeOperatorPrivilege(std::string username)
{
	std::map<int,std::string>::iterator it = _operators.begin();
	std::map<int,std::string>::iterator ite = _operators.end();
	for(; it != ite; it++)
	{
		if (it->second == username)
		{
			_operators.erase(it);
			break;
		}
	}
}

void	Channel::addOperatorPrivilege(std::string username)
{
	(void)username;
}

// std::string				Channel::getChannelTopic()
// {
// 	return (_channel_topic);
// }

std::string				Channel::getPassword()
{
	return (_key);
}

bool					Channel::isModeOn(char mode)
{
	int r_value = 0;
	switch (mode)
	{
	case 'i':
		r_value = _invite_only;
		break;
	case 't':
		r_value = _topic_protection;
		break;
	case 'k':
		if (_key != "")
			r_value = 1;
		break;
	case 'l':
		if (_limit_user != 0)
			r_value = 1;
		break;		
	default:
		break;
	}
	return (r_value);
}

bool					Channel::isOperator(int socket_user)
{
	if (_operators.find(socket_user) != _operators.end())
		return true;
	else 
		return false;
}

/*
bool 0 : off
bool 1 : on
*/
void	Channel::setUnsetInviteMode(bool on_off)
{
	_invite_only = on_off;
}

void	Channel::setUnsetTopicRestr(bool on_off)
{
	_topic_protection = on_off;
}

void	Channel::setUnsetPassword(bool on_off, std::string password)
{
	if (on_off)
		_key = password;
	else
		_key = "";
}

void	Channel::setUnsetUserLimit(bool on_off, size_t user_limit)
{
	if (on_off)
		_limit_user = user_limit;
	else
		_limit_user = 0;
}

void	Channel::setUnsetOpPrivilege(bool on_off, std::string username)
{
	if (on_off)
		addOperatorPrivilege(username);
	else
		removeOperatorPrivilege(username);
}

void	Channel::setTopicName(std::string new_topic_name)
{
	_channel_topic = new_topic_name;
}

size_t	Channel::getUserLimit()
{
	return (_limit_user);
}

void	Channel::setName(std::string name)
{
	this->_name = name;
}

std::string Channel::getName()
{
	return this->_name;
}

void	Channel::printUsersInChannel(Client* client, std::string& channel_name)
{
	std::cout << "Users in the channel : ";
	std::string msg = "Users in " + channel_name + " :";
	sendMessageToClient(client->getSocket(), msg);
	for(std::map<std::string, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		msg = it->first + "\n";
		sendMessageToClient(client->getSocket(), msg);
	}
}

bool	Channel::hasUser(std::string nickname)
{
	std::cout << nickname << std::endl;
	for(std::map<std::string, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		std::cout << "Map it->first" << it->first << std::endl;
		if(it->first == nickname)
			return (true);
	}
	std::cout << "false" << std::endl;
	return (false);
}