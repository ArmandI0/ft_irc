#include "Channel.hpp"

Channel::Channel(const std::string& topic, Client* creator, Server* serv):_channel_topic(topic), _server(serv), _password(""), _user_limit(0), _topic_op_mode(0)
{
	(void)creator;
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
	if(!client)
		return ;
	std::string nickname = client->getNick();
	for (std::map<std::string, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		// Check if the nickname matches any key in the map
		if (it->first == nickname)
		{
			std::cout << "Client with nickname " << nickname << " is already in the map." << std::endl;
			return ;
		}
	}
	std::cout << "Adding new client to the map." << std::endl;
	_clients.insert(std::make_pair(nickname, client));
	std::cout << "Client added" << std::endl;
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
	_password = src._password;
	_user_limit = src._user_limit;
	_topic_op_mode = src._topic_op_mode;
	_invite_mode = src._invite_mode;
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
	return (_password);
}

bool					Channel::isModeOn(char mode)
{
	int r_value = 0;
	switch (mode)
	{
	case 'i':
		r_value = _invite_mode;
		break;
	case 't':
		r_value = _topic_op_mode;
		break;
	case 'k':
		if (_password != "")
			r_value = 1;
		break;
	case 'l':
		if (_user_limit != 0)
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
	_invite_mode = on_off;
}

void	Channel::setUnsetTopicRestr(bool on_off)
{
	_topic_op_mode = on_off;
}

void	Channel::setUnsetPassword(bool on_off, std::string password)
{
	if (on_off)
		_password = password;
	else
		_password = "";
}

void	Channel::setUnsetUserLimit(bool on_off, size_t user_limit)
{
	if (on_off)
		_user_limit = user_limit;
	else
		_user_limit = 0;
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
	return (_user_limit);
}

void	Channel::setName(std::string name)
{
	this->_name = name;
}

std::string Channel::getName()
{
	return this->_name;
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