/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:48:29 by aranger           #+#    #+#             */
/*   Updated: 2024/08/27 17:20:56 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int sig = 1;

void handle_sigint(int s)
{
	(void)s;
	sig = 0;
}

Server::Server(unsigned long port, std::string password) :  _password(password)
{
	this->_listen_socket = -1;
	this->_epoll_socket = -1;
	this->_server_infos.sin_port = htons(port);
	this->_server_infos.sin_family = AF_INET;
	this->_server_infos.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

Server::~Server()
{
	if (this->_listen_socket != -1)
		close(this->_listen_socket);
	if (this->_epoll_socket != -1)
		close(this->_epoll_socket);
	this->closeClientsFd();
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		delete it->second;
	}
}

void    Server::listenSocket()
{
	int status;
	int log = 10;

	this->_listen_socket = socket(this->_server_infos.sin_family, SOCK_STREAM, 0);
	if (this->_listen_socket == -1)
		throw std::runtime_error("Error : socket");
	status = bind(this->_listen_socket, (struct sockaddr *)&(this->_server_infos), sizeof this->_server_infos);
	if (status != 0)
		throw std::runtime_error("Error : socket already use");
	status = listen(this->_listen_socket, log);
	if (status != 0)
		throw std::runtime_error("Error : listen socket fail");
}

void	Server::execServer()
{
	struct epoll_event ev;
	struct epoll_event evs[10];
	int ret;

	ev.events = EPOLLIN;
	ev.data.fd = this->_listen_socket;
    std::signal(SIGINT, handle_sigint);
	std::signal(SIGQUIT, handle_sigint);
	this->_epoll_socket = epoll_create1(0);
	if (this->_epoll_socket == -1)
		throw std::runtime_error("Error : epoll_create fail.");
	if (epoll_ctl(this->_epoll_socket, EPOLL_CTL_ADD, this->_listen_socket, &ev) == -1)
		throw std::runtime_error("Error : epoll_ctl fail.");
	std::cout << MAGENTA << "Server run"  << RESET << std::endl;
	while(sig)
	{
		ret = epoll_wait(this->_epoll_socket, evs, 10, -1);
		if (ret == -1)
		{
			if (sig == 0)
				return ;
			throw std::runtime_error("Error : epoll_wait fail.");
		}
		for (int i = 0; i < ret; i++)
		{
			if (evs[i].events & EPOLLIN)
			{
				if (evs[i].data.fd == this->_listen_socket)
				{
					int new_client_fd = accept(this->_listen_socket, NULL, NULL);
					if (new_client_fd == -1)
						throw std::runtime_error("Error : accept new fd.");
					epoll_event new_ev = this->addNewClient(new_client_fd);
					if(epoll_ctl(this->_epoll_socket, EPOLL_CTL_ADD, new_client_fd, &new_ev) == -1)
						throw std::runtime_error("Error : epoll_ctl fail.");
                }
                else
					execCommand(this->_users[evs[i].data.fd]);	
			}
		}
	}
}

void	Server::execCommand(Client & client)
{
	std::string buffer = readSocket(client.getSocket());
	if (!buffer.empty())
	{
		if (!buffer.empty())
		{
			client.setEntry(buffer);
			if (client.getEntry().find("\n") != std::string::npos)
			{
				Command	new_command(client.getEntry(), &client, &*this);
				client.eraseEntry();
				new_command.parsingCommand();
			}
		}
	}
}

std::string	Server::getPassword()
{
	return this->_password;
}

std::string Server::readSocket(int fd)
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
        this->delClient(fd);
        return "";
    }
    else
    {
        std::cerr << "Error : message" << fd << std::endl;
        return "";
    }
}

void	Server::closeClientsFd()
{
	for (std::map<int, Client>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		close(it->first);
	}
}
