/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboire <dboire@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:48:29 by aranger           #+#    #+#             */
/*   Updated: 2024/08/22 23:15:43 by dboire           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


void handle_sigint(int sig)
{
	(void)sig;
}

Server::Server(std::string port, std::string password) :  _password(password)
{
	this->_server_infos.sin_port = htons(std::atoi(port.c_str()));
	this->_server_infos.sin_family = AF_INET;
	this->_server_infos.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

Server::~Server()
{
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		delete it->second; // Delete all dynamically allocated Channels
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
		throw std::runtime_error("Error : bind socket");
	status = listen(this->_listen_socket, log);
	if (status != 0)
	{
		std::cerr << "Error TBD"<< std::endl;
		return ;
	}
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
	std::cout << (this->_users[client_fd]).getUsername() << " left the channel." << std::endl;
	this->_users.erase(client_fd);
}

void	Server::execServer()
{
	struct epoll_event ev;
	ev.events = EPOLLIN;  // Par exemple, surveiller les événements de lecture (EPOLLIN)
	ev.data.fd = this->_listen_socket;
	
    std::signal(SIGINT, handle_sigint);
	
	/* CREATION EPOLL*/
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

	std::cout << "Waiting connection..." << std::endl;
	struct epoll_event evs[10];
	int ret;
	while(1)
	{
		ret = epoll_wait(this->_epoll_socket, evs, 10, -1);
		if (ret == -1)
		{
			close(this->_epoll_socket);
    		close(this->_listen_socket);
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < ret; i++)
		{
			if (evs[i].events & EPOLLIN)
			{
				if (evs[i].data.fd == this->_listen_socket)
				{
					int new_client_fd = accept(this->_listen_socket, NULL, NULL);
					if (new_client_fd == -1)
						std::cerr << "Error TBD" << std::endl;
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
        std::cerr << "Erreur lors de la réception de données du fd " << fd << std::endl;
        return "";
    }
}



void Server::addNewNickname(std::string & nick, Client * client)
{
	this->_nicknames.insert(std::make_pair(nick, client));
}

Client*	Server::findUserByNickname(std::string & nickname)
{
	std::map<std::string,Client*>::iterator it = this->_nicknames.find(nickname);
	if (it == this->_nicknames.end())
		return NULL;
	return it->second;
}

void	Server::addNewUsername(std::string & username, Client * client)
{
	this->_usernames.insert(std::make_pair(username, client));
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
	std::map<std::string,Client*>::iterator it = this->_usernames.find(nickname);
	this->_nicknames.erase(it);
}

void	Server::delClientByUsername(std::string & username)
{
	std::map<std::string,Client*>::iterator it = this->_usernames.find(username);
	this->_usernames.erase(it);
}

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
