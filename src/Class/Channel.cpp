#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(const std::string& topic, Client& creator, Server* serv):_channel_topic(topic), _channel_mode(""), _server(serv)
{
	int socket = creator.getSocket();
	_clients[socket] = (&creator);
	_operators[socket] = (&creator);
}

Channel::Channel(const Channel& src)
{
	*this = src;
}

Channel::~Channel()
{
}

void	Channel::addClient(Client& client)
{
	_clients[client.getSocket()] = (&client);
}

void	Channel::delClient(Client& client)
{
	int socket = client.getSocket();
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
	_channel_mode = src._channel_mode;
	_channel_topic = src._channel_topic;
	_clients = src._clients;
	_operators = src._operators;
	_server = src._server;
	return (*this);
}

/*	changeChannelMode(std::string& str)
	Takes a str that represents the different modes, for example : "i+t-k+o+l+" or just "i-t-k-"
	if + : add (or do nothing if already present)
	if - : erase (or do nothing if already absent)
*/
void					Channel::changeChannelMode(std::string& str)
{
	std::string::iterator it = str.begin();
	std::string::iterator ite = str.end();
	for(; it != ite; it++)
	{
		if (*it == '+' || *it == '-')
			continue;
		else if (it + 1 == ite)
			break;
		else if (*(it+1) == '+' && _channel_mode.find(*it) == std::string::npos)
			_channel_mode+=*it;
		else if (*(it+1) == '-' && _channel_mode.find(*it) != std::string::npos)
			_channel_mode.erase(_channel_mode.find(*it), 1);
	}
}

std::map<int,Client*>	Channel::getClients()
{
	return (_clients);
}

std::map<int,Client*>	Channel::getOperators()
{
	return (_operators);
}

std::string				Channel::getChannelTopic()
{
	return (_channel_topic);
}

std::string				Channel::getChannelModes()
{
	return (_channel_mode);
}

bool					Channel::isModeOn(char mode)
{
	if (_channel_mode.find_first_of(mode) != std::string::npos)
		return (true);
	else
		return (false);
}
