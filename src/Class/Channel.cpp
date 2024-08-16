#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(const std::string& topic, Client& creator, Server* serv):_channel_topic(topic), _channel_mode(""), _server(serv)
{
	_clients.push_back(&creator);
	_operators.push_back(&creator);
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
	(void)client;
}

void	Channel::delClient(Client& client)
{
	(void)client;
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
	return (*this);
}