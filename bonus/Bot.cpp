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

std::string Bot::strToLower(std::string& str)
{
	std::string str_to_lower = str;
	std::string::iterator it = str_to_lower.begin();
	std::string::iterator ite = str_to_lower.end();

	for (; it != ite; it++)
		*it = tolower(*it);
	return str_to_lower;
}

void	Bot::getInsults()
{
	std::ifstream file;
	std::string line;

	file.open(INSULTS);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			if (!file.eof())
				_insults.push_back(strToLower(line));
		}
		file.close();
	}
	else
		throw std::runtime_error("open file error : " + std::string(INSULTS));
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

void	Bot::connectBot()
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
	authentification();
	createBotChannel();
}


void	Bot::loopBot()
{
	struct epoll_event 	event;
	_epoll_socket = epoll_create1(0);

	if (_epoll_socket == -1)
		throw std::runtime_error("epoll_create() failed with error: " + std::string(strerror(errno)));

	event.events=EPOLLIN;
	event.data.fd=_server_socket;

	if (epoll_ctl(_epoll_socket, EPOLL_CTL_ADD, _server_socket, &event) == -1)
		throw std::runtime_error("epoll_ctl() failed with error: " + std::string(strerror(errno)));
	
	waitingEvents();
}

void Bot::waitingEvents()
{
	std::string 	 	response = "";
	std::string 	 	buffer = "";
	struct epoll_event	events[10];
	int 				nb_fds = epoll_wait(_epoll_socket, events, 10, -1);
	
	while (true)
	{
		if (nb_fds == -1)
			throw std::runtime_error("epoll_wait() failed with error: " + std::string(strerror(errno)));
		for (int i = 0; i < nb_fds; i++)
		{
			if (events[i].events & EPOLLIN)
			{
				buffer = readSocket(events[i].data.fd);
				response = buffer;
				//std::cout << "server '" << buffer << "'" << std::endl;
				if (response.find('\n') == std::string::npos)
					response += buffer;
				else
				{
					botResponse(response);
					response = "";
				}
			}
		}		
	}
}

std::string Bot::readSocket(int fd)
{
    char buffer[4096];
    ssize_t bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_read > 0)
    {
        buffer[bytes_read] = '\0';
        return std::string(buffer);
    }
    else if (bytes_read == 0)
    {
        close(fd);
        epoll_ctl(_epoll_socket, EPOLL_CTL_DEL, fd, NULL);
        return "";
    }
    else
    {
		throw std::runtime_error("recv() failed with error: " + std::string(strerror(errno)));
        return "";
    }
}

void 	Bot::	authentification()
{
	size_t bytes_sent = 0;
	std::vector<std::string> auth_bot_cmds;
	auth_bot_cmds.push_back(std::string("PASS ") + _server_pass + "\r\n");
	auth_bot_cmds.push_back(std::string("NICK ") + BOT + "\r\n");
	auth_bot_cmds.push_back(std::string("USER ") + BOT + " bot * :realname " + "\r\n");

	for (int i = 0; i < 3; i++)
	{
		bytes_sent = send(_server_socket, auth_bot_cmds[i].c_str(), auth_bot_cmds[i].size(), 0);
		if (bytes_sent != auth_bot_cmds[i].size())
			throw std::runtime_error("send() " + auth_bot_cmds[i].substr(0,4)+ " failed with error: " + std::string(strerror(errno)));
	}
}

void	Bot::botResponse(std::string& request)
{
	std::vector<std::string> elements;
	std::istringstream str_stream(request);
	std::string element;
	std::string	author = "";
	std::string response = "";

	while (std::getline(str_stream, element, ' '))
		elements.push_back(element);

	author = elements[0];
	if (author[0] == ':')
		author.erase(0,1);

	if (elements[1] == "PRIVMSG" && elements[2] == BOT)
	{
		if (checkIfMean(request))
			response = std::string("PRIVMSG ") + author + " You're mean !";
		else
			response = std::string("PRIVMSG ") + author + " You're nice !";
	}
	else if (elements[1] == "PRIVMSG" && elements[2] == "#politesse")
	{
		if (checkIfMean(request))
			response = std::string("KICK #politesse ") + author +" \r\n";
		else
			return;
	}
	else
		return;
	size_t bytes_sent = 0;
	bytes_sent = send(_server_socket, response.c_str(), response.size(), 0);
	if (bytes_sent != response.size())
		throw std::runtime_error("send() " + response.substr(0,4)+ " failed with error: " + std::string(strerror(errno)));		
}

bool Bot::checkIfMean(std::string& str)
{
	std::string str_to_lower = strToLower(str);
	std::vector<std::string>::iterator it = _insults.begin();
	std::vector<std::string>::iterator ite = _insults.end();
	for (; it != ite; it++)
	{
		if (str_to_lower.find(*it) != std::string::npos)
			return (1);
	}
	return (0);
}

void	Bot::createBotChannel()
{
	size_t bytes_sent = 0;
	std::string create_channel = std::string("JOIN #politesse \r\n");

    bytes_sent = send(_server_socket, create_channel.c_str(), create_channel.size(), 0);
	if (bytes_sent != create_channel.size())
		throw std::runtime_error("send() JOIN #politesse failed with error: " + std::string(strerror(errno)));	
}
