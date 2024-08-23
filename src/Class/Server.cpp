/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:48:29 by aranger           #+#    #+#             */
/*   Updated: 2024/08/23 11:23:22 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int sig = 1;

void handle_sigint(int s)
{
	(void)s;
	sig = 0;
}

Server::Server(std::string port, std::string password) :  _password(password)
{
	this->_listen_socket = -1;
	this->_server_infos.sin_port = htons(std::atoi(port.c_str()));
	this->_server_infos.sin_family = AF_INET;
	this->_server_infos.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

Server::~Server()
{
	if (this->_listen_socket != -1)
		close(this->_listen_socket);
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
	{
		throw std::runtime_error("Error : socket already use");
	}
	status = listen(this->_listen_socket, log);
	if (status != 0)
		throw std::runtime_error("Error : socket");
}

epoll_event	Server::addNewClient(int new_client_fd)
{
	struct epoll_event	new_ev;
	Client				new_client = Client(new_client_fd);		

	new_ev.events = EPOLLIN;
	new_ev.data.fd = new_client_fd;
	this->_users.insert(std::make_pair(new_client_fd, new_client));
	new_client.setSocket(new_client_fd);
	return (new_ev);
}

void	Server::delClient(int client_fd)
{
	Client to_delete = getClientByFd(client_fd);

	std::map<std::string,Client*>::iterator it = this->_nicknames.find(to_delete.getNick());
	if (it != _nicknames.end())
		this->_nicknames.erase(it);

	it = this->_usernames.find(to_delete.getUsername());
	if (it != _usernames.end())
		this->_usernames.erase(it);

	this->_users.erase(client_fd);

	std::cout << "Client : " << client_fd << " left the server." << std::endl;
}

void	Server::execServer()
{
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = this->_listen_socket;
    std::signal(SIGINT, handle_sigint);
	
	this->_epoll_socket = epoll_create1(0);
	if (this->_epoll_socket == -1)
	{
		std::cerr << "Error TBD"<< std::endl;
		exit(EXIT_FAILURE); 
	}

	/* AJOUT FD A SURVEILLER TO EPOLL */
	if (epoll_ctl(this->_epoll_socket, EPOLL_CTL_ADD, this->_listen_socket, &ev) == -1)
	{
		std::cerr << "Error TBD"<< std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << MAGENTA "Server run" RESET << std::endl;
	struct epoll_event evs[3];
	int ret;
	while(sig)
	{
		ret = epoll_wait(this->_epoll_socket, evs, 3, -1);
		if (ret == -1)
		{
			break;
		}
		for (int i = 0; i < ret; i++)
		{
			if (evs[i].events & EPOLLIN)
			{
				if (evs[i].data.fd == this->_listen_socket)
				{
					int new_client_fd = accept(this->_listen_socket, NULL, NULL);
					if (new_client_fd == -1)
						std::cerr << "Error : blabla" << std::endl;
					else
					{
						std::cout << "connexion etablie fd =" << new_client_fd << std::endl;

						epoll_event new_ev = this->addNewClient(new_client_fd);
						if(epoll_ctl(this->_epoll_socket, EPOLL_CTL_ADD, new_client_fd, &new_ev) == -1)
						{
							std::cerr << "Error TBD"<< std::endl;
							exit(EXIT_FAILURE);
						}
                    }
                }
                else
                {
					execCommand(this->_users[evs[i].data.fd]);	
				}
			}
		}
	}
	closeClientsFd();
	close(this->_epoll_socket);
    close(this->_listen_socket);
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
				std::cout << "Buffer read :\n" RED << buffer  << RESET "Buffer read :" << std::endl;
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
        epoll_ctl(this->_epoll_socket, EPOLL_CTL_DEL, fd, NULL);
        this->delClient(fd);
        return "";
    }
    else
    {
        std::cerr << "Error : message" << fd << std::endl;
        return "";
    }
}



void Server::addNewNickname(std::string & nick, Client * client)
{
	this->_nicknames.insert(std::make_pair(nick, client));
}

void	Server::addNewUsername(std::string & username, Client * client)
{
	this->_usernames.insert(std::make_pair(username, client));
}

Client*	Server::findUserByNickname(std::string & nickname)
{
	std::map<std::string,Client*>::iterator it = this->_nicknames.find(nickname);
	if (it == this->_nicknames.end())
		return NULL;
	return it->second;
}


Client*	Server::findUserByUsername(std::string & username)
{
	std::map<std::string,Client*>::iterator it = this->_usernames.find(username);
	if (it == this->_usernames.end())
		return NULL;
	return it->second;
}

Client&		Server::getClientByFd(int socket)
{
	Client& ref = _users[socket];
	return (ref);
}


void	Server::delClientByNickname(std::string & nickname)
{
	std::map<std::string,Client*>::iterator it = this->_nicknames.find(nickname);
	if (it != _nicknames.end())
		this->_nicknames.erase(it);
}

void	Server::delClientByUsername(std::string & username)
{
	std::map<std::string,Client*>::iterator it = this->_usernames.find(username);
	if (it != _usernames.end())
		this->_usernames.erase(it);
}

void Server::setChannel(Channel & channel, std::string & channel_name)
{
	this->_channels[channel_name] = channel;
}

void	Server::delChannel(std::string& channel_name)
{
	std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
	if(it != _channels.end())
	_channels.erase(it);
}

Channel*	Server:: getChannel(std::string& channel_name)
{
	std::map<std::string,Channel>::iterator it  = this->_channels.find(channel_name);
	if(it != _channels.end())
		return (&it->second);
	std::cout << "The channel : " << channel_name << " does not exist" << std::endl;
	return(NULL);
}

void	Server::printChannels()
{
	std::map<std::string, Channel>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); it++)
		std::cout << it->first << std::endl;
}

void	Server::closeClientsFd()
{
	for (std::map<int, Client>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		close(it->first);
	}
}
