#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(const std::string& topic, Client& creator):_channel_topic(topic), _channel_mode("")
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
}
void	Channel::delChannel()
{
}

Channel& Channel::operator=(const Channel& src)
{
	_channel_mode = src._channel_mode;
	_channel_topic = src._channel_topic;
	_clients = src._clients;
	_operators = src._operators;
	return (*this);
}