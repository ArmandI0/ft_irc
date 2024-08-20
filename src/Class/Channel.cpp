#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(const std::string& topic, Client& creator, Server* serv):_channel_topic(topic), _server(serv), _password(""), _user_limit(0), _invite_mode(0), _topic_op_mode(0)
{
	int socket = creator.getSocket();
	_clients[socket] = creator.getUsername();
	_operators[socket] = creator.getUsername();
}

Channel::Channel(const Channel& src)
{
	*this = src;
}

Channel::~Channel()
{
}

void	Channel::addClient(Client* client)
{
	_clients[client->getSocket()] = client->getUsername();
}

void	Channel::delClient(Client* client)
{
	int socket = client->getSocket();
	if (_clients.find(socket) != _clients.end())
		_clients.erase(socket);
	if (_clients.size() == 0)
		delChannel();
}

void	Channel::delChannel()
{
	_server->delChannel(this->_channel_topic);
}

Channel& Channel::operator=(const Channel& src)
{
	//_channel_mode = src._channel_mode;
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

/*	changeChannelMode(std::string& str)
	Takes as first arg a str that represents the different modes, for example : "+i-t+k+o+l" or just "-i-t-k"
	Takes as second arg a vector that contains arguments if necessary (in ordrer)
	if + : add (or do nothing if already present)
	if - : erase (or do nothing if already absent)
*/
/* void	Channel::changeChannelMode(std::string& str, std::vector<std::string> args)
{
	std::string::iterator it = str.begin();
	std::string::iterator ite = str.end();
	int	arg_nb = 0;
	for(; it != ite; it++)
	{
		if (*it == '+' || *it == '-')
			continue;
		else if (*(it-1) == '+' && _channel_mode.find(*it) == std::string::npos)
		{
			if (*it != 'o')
				_channel_mode+=*it;
			else if (*it == 'o')
			{
				addOperatorPrivilege(args[arg_nb]);
				arg_nb++;
			}
		}
		else if (*(it-1) == '-' && _channel_mode.find(*it) != std::string::npos)
		{
				_channel_mode.erase(_channel_mode.find(*it), 1);
		}
		else if (*(it-1) == '-' && *it == 'o')
		{
			removeOperatorPrivilege(args[arg_nb]);
			arg_nb++;
		}
	}
} */

void	Channel::removeOperatorPrivilege(std::string username)
{
	std::map<int,std::string>::iterator it = _operators.begin();
	std::map<int,std::string>::iterator ite = _operators.end();
	for(; it != ite; it++)
	{
		if (it->second == username)
			_operators.erase(it->first);
	}
}

void	Channel::addOperatorPrivilege(std::string username)
{
	std::map<int,std::string>::iterator it = _operators.begin();
	std::map<int,std::string>::iterator ite = _operators.end();
	for(; it != ite; it++)
	{
		if (it->second == username)
			return;
	}
	int user_socket =_server->getClientFdByUsername(username);
	if (user_socket != -1)
		_operators[user_socket] = username;
}

std::map<int,std::string>	Channel::getClientsList()
{
	return (_clients);
}

std::map<int,std::string>	Channel::getOperatorsList()
{
	return (_operators);
}

std::string				Channel::getChannelTopic()
{
	return (_channel_topic);
}

/* std::string				Channel::getChannelModes()
{
	return (_channel_mode);
} */

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
		user_limit = 0;
}

void	Channel::setUnsetOpPrivilege(bool on_off, std::string username)
{
	if (on_off)
		addOperatorPrivilege(username);
	else
		removeOperatorPrivilege(username);
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