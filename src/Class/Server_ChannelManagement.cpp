/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_ChannelManagement.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:30:32 by aranger           #+#    #+#             */
/*   Updated: 2024/08/26 15:30:46 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::setChannel(Channel * channel, std::string & channel_name)
{
	this->_channels[channel_name] = channel;
}

void	Server::delChannel(std::string& channel_name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(channel_name);
	if(it != _channels.end())
	_channels.erase(it);
}

Channel*	Server:: getChannel(std::string& channel_name)
{
	std::map<std::string,Channel *>::iterator it  = this->_channels.find(channel_name);
	if(it != _channels.end())
		return (it->second);
	std::cout << "The channel : " << channel_name << " does not exist" << std::endl;
	return(NULL);
}

void	Server::printChannels()
{
	std::map<std::string, Channel*>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); it++)
		std::cout << it->first << std::endl;
}