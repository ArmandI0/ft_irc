/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:48:29 by aranger           #+#    #+#             */
/*   Updated: 2024/08/20 17:47:26 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot(char* ip, char* pass, char *port): _server_ip(ip), _server_pass(pass), _server_port(port), _res(NULL)
{
}

Bot::Bot(const Bot& src)
{
	*this = src;
}

Bot& Bot::operator=(const Bot& src)
{
	_server_ip = src._server_ip;
	_server_pass = src._server_pass;
	_server_port = src._server_port;
	_server_socket = src._server_socket;
	return(*this);
}

Bot::~Bot()
{
	freeaddrinfo(_res);
}

void	Bot::connect_bot()
{
	int status;
	struct addrinfo hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(_server_ip, _server_port, &hints, &_res);
	if (status != 0)
		throw std::runtime_error("getaddrinfo() failed with error code: " + std::to_string(status));

	_server_socket = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
	if (_server_socket == -1)
		throw std::runtime_error("socket() failed with error: " + std::string(strerror(errno)));

	status = connect(_server_socket, _res->ai_addr, _res->ai_addrlen);
	if (status != 0)
		throw std::runtime_error("connect() failed with error: " + std::string(strerror(errno)));
}


