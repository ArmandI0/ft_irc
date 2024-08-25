/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboire <dboire@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 21:07:07 by dboire            #+#    #+#             */
/*   Updated: 2024/08/25 20:27:36 by dboire           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(){};
Channel::Channel(const Channel& src){*this = src;};
Channel::~Channel(){};

Channel::Channel(std::string & name, Client* creator, Server* serv):  _name(name), _server(serv), _key(""), _limit_user(-1), _invite_only(false), _topic_protection(false), _channel_topic("No topic is set\r\n")
{
	sendMessageToClient(creator->getSocket(), ERR_NOTOPIC(creator->getNick(), this->getName()));
	addClientToCh(creator);
	addClientToOp(creator);
}

void	Channel::addClientToOp(Client* client)
{
	if(checkIfOp(client->getNick()) == true)
		return ;
	_operator.insert(std::make_pair(client->getNick(), client));
}

void	Channel::delClientToOp(Client* client)
{
	for(std::map<std::string, Client *>::iterator it = _operator.begin(); it != _operator.end(); ++it)
	{
		if(it->first == client->getNick())
		{
			_clients.erase(it);
			break ;
		}
	}
}

bool	Channel::checkLimitUser()
{
	size_t i = 0;
	if(this->getLimitUser() > 0)
	{
		for(std::map<std::string, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
			i++;
		if(i >= this->getLimitUser())
			return(true);
	}
	return(false);
}

bool	Channel::checkInvite(std::string name)
{
	if(this->_invite_only == true)
	{
		for(std::vector<std::string>::iterator it = _invite_name.begin(); it != _invite_name.end(); ++it)
		{
			if(*it == name)
				return (true);	
		}
		return (false);
	}
	return (true);
}

void	Channel::addClientToCh(Client* client)
{
	_clients.insert(std::make_pair(client->getNick(), client));
	sendMessageToClient(client->getSocket(),":" + client->getNick() + " JOIN " + this->getName() + "\r\n");
	printUsersInChannel(client, this->_name);
	if(_clients.size() > 1)
		notifyJoin(client->getNick());
}

void	Channel::notifyJoin(std::string nickname)
{
	for(std::map<std::string, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		sendMessageToClient(it->second->getSocket(), MSG_WELCOME(nickname, this->getName()));
}

void	Channel::sendMessageToAllClient(std::string error)
{
	for(std::map<std::string, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		sendMessageToClient(it->second->getSocket(), error);
}

void	Channel::sendMessageToAllClient(std::string sender, std::string message)
{
	for(std::map<std::string, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->first != sender)
			sendMessageToClient(it->second->getSocket(), message);
	}
}

void	Channel::kickClient(Client* client, std::string target, std::string reason)
{
	(void)reason;
	for(std::map<std::string, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if(it->first == target)
		{
			sendMessageToClient(it->second->getSocket(),":" + client->getNick() + " KICK " + this->getName() + " " + target + " :" + client->getNick() + "\r\n");
			_clients.erase(it);
			break ;
		}
	}
	for(std::map<std::string, Client *>::iterator it = _operator.begin(); it != _operator.end(); ++it)
	{
		if(it->first == target)
		{
			_clients.erase(it);
			break ;
		}
	}
}

void	Channel::delClient(std::string client)
{
	this->_clients.erase(client);
}

void	Channel::delChannel()
{
	_server->delChannel(this->_channel_topic);
}

Channel& Channel::operator=(const Channel& src)
{
	_channel_topic = src._channel_topic;
	_clients = src._clients;
	_operator = src._operator;
	_server = src._server;
	_key = src._key;
	_limit_user = src._limit_user;
	_topic_protection = src._topic_protection;
	_invite_only = src._invite_only;
	return (*this);
}

// std::string				Channel::getChannelTopic()
// {
// 	return (_channel_topic);
// }

std::string				Channel::getKey()
{
	return (_key);
}

void	Channel::addClientToInvite(Client * client, Client * t_client)
{
	for(std::vector<std::string>::iterator it = _invite_name.begin(); it != _invite_name.end(); ++it)
	{
		if(*it == t_client->getNick())
			return;
	}
	_invite_name.push_back(t_client->getNick());
	sendMessageToClient(t_client->getSocket(), MSG_INVITEE(client->getNick(), t_client->getNick(), this->getName()));
	sendMessageToClient(client->getSocket(), MSG_INVITER(client->getNick(), t_client->getNick(), this->getName()));
}

size_t	Channel::getUserLimit()
{
	return (_limit_user);
}

std::string Channel::getName()
{
	return this->_name;
}

std::string Channel::getTopic()
{
	return (this->_channel_topic);
}

bool	Channel::getTopicProtection()
{
	return (this->_topic_protection);
}

size_t	Channel::getLimitUser()
{
	return (this->_limit_user);
}

bool	Channel::getInvite()
{
	return(this->_invite_only);
}

void	Channel::printUsersInChannel(Client* client, std::string& channel_name)
{
	std::string msg;
	
	sendMessageToClient(client->getSocket(), RPL_NAMREPLY(client->getNick(),channel_name));
	for(std::map<std::string, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		msg = it->first + " ";
		sendMessageToClient(client->getSocket(), msg);
	}
	// sendMessageToClient(client->getSocket(), "\n");
	// sendMessageToClient(client->getSocket(), ERR_ENDOFNAMES(client->getNick(), channel_name));
}

bool	Channel::hasUser(std::string nickname)
{
	std::cout << nickname << std::endl;
	for(std::map<std::string, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if(it->first == nickname)
			return (true);
	}
	return (false);
}

void	Channel::setKey(std::string key)
{
	this->_key = key;
}

void	Channel::setTopicMsg(std::string topic)
{
	this->_channel_topic = topic;
}

void	Channel::setLimit(std::string limit)
{
	std::stringstream ss(limit);
	size_t limit_value;
	ss >> limit_value;
	this->_limit_user = limit_value;
}

void	Channel::setTopic(int remove)
{
	if(remove == true)
		this->_topic_protection = false;
	else
		this->_topic_protection = true;
}

void	Channel::setInvite(int remove)
{
	if(remove == true)
		this->_invite_only = false;
	else
		this->_invite_only = true;
}
