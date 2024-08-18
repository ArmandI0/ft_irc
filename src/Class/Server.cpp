/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:48:29 by aranger           #+#    #+#             */
/*   Updated: 2024/08/18 18:44:35 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string port, std::string password) :  _password(password)
{
	this->_server_infos.sin_port = htons(std::atoi(port.c_str()));
	this->_server_infos.sin_family = AF_INET;
	this->_server_infos.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

Server::~Server()
{
	
}

void    Server::listenSocket()
{
	int status;
	int log = 10;	//nb max de connexion

	/* CREER LE SOCKET D'ECOUTE */

	this->_listen_socket = socket(this->_server_infos.sin_family, SOCK_STREAM, 0);
	if (this->_listen_socket == -1)
	{
		std::cerr << "Error TBD"<< std::endl;
		return ;
	}

	/* ECOUTE SUR LE SOCKET CREE */
    
	status = bind(this->_listen_socket, (struct sockaddr *)&(this->_server_infos), sizeof this->_server_infos);
	if (status != 0)
	{
		std::cerr << "Error TBD"<< std::endl;
		return ;
	}
	status = listen(this->_listen_socket, log);
	if (status != 0)
	{
		std::cerr << "Error TBD"<< std::endl;
		return ;
	}
}	

void	Server::clientAuth(Client & client)
{
	(void)client;
}

epoll_event	Server::addClient(int new_client_fd)
{
	struct epoll_event	new_ev;
	Client				new_client = Client(new_client_fd);		

	new_ev.events = EPOLLIN;
	new_ev.data.fd = new_client_fd;
	this->_users.insert(std::make_pair(new_client_fd, new_client));
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
			std::cerr << "Error TBD"<< std::endl;
			exit(EXIT_FAILURE);
		}
		/* gestion des events */
		for (int i = 0; i < ret; i++)
		{
			if (evs[i].events & EPOLLIN)
			{
				/* Si events sur le socket d'entree du serveur : accepter la nouvelle connection */
				if (evs[i].data.fd == this->_listen_socket)
				{
					int new_client_fd = accept(this->_listen_socket, NULL, NULL);
					if (new_client_fd == -1)
						std::cerr << "Error TBD" << std::endl;
					else
					{
						std::cout << "connexion etablie fd =" << new_client_fd << std::endl;

						epoll_event new_ev = this->addClient(new_client_fd);
                        if(epoll_ctl(this->_epoll_socket, EPOLL_CTL_ADD, new_client_fd, &new_ev) == -1)
                        {
                            std::cerr << "Error TBD"<< std::endl;
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                else
                {
                    /* PAS D"AUTH POUR L'INSTANT */
                    char buffer[1024];
                    ssize_t bytes_read = recv(evs[i].data.fd, buffer, sizeof(buffer) - 1, 0);
                    if (bytes_read > 0)
					{
                        buffer[bytes_read] = '\0';
                        std::cout << "Reçu du fd " << evs[i].data.fd << ": " << buffer << std::endl;
					}
					if (bytes_read == 0)
					{
						close(evs[i].data.fd);
						epoll_ctl(this->_epoll_socket, EPOLL_CTL_DEL, evs[i].data.fd, NULL);
						this->delClient(evs[i].data.fd);
					}
					std::string a(buffer);
					Command	new_command(a, &(this->_users[evs[i].data.fd]), &*this);
					
                    if((this->_users[evs[i].data.fd]).getAuth() == false)
                    {
                        std::cout << "User = " << evs[i].data.fd << "Non conecte" << std::endl;
						new_command.server_msg();
                    }
                    else
                    {
                        std::cout << "User = " << evs[i].data.fd << "CONNECTE" << std::endl;
                    }                    
				}
			}
		}
	}
}

void	Server::delChannel(std::string& channel_name)
{
	_channels.erase(channel_name);
}

void	Server::createChannel(std::string& channel_name, Client& client_creator)
{
	Channel new_channel(channel_name, client_creator, this);
	_channels.insert(std::pair<std::string, Channel>(channel_name, new_channel));
}

void	Server::print_list_channels()
{
	std::map<std::string,Channel>::iterator it = _channels.begin();
	std::map<std::string,Channel>::iterator ite = _channels.end();
	for(; it != ite; it++)
	{
		std::cout << it->first << std::endl;
	}
}

std::map<std::string,Channel>&		Server::getChannels()
{
	return _channels;
}

Channel*							Server::getChannelByTopic(std::string topic)
{
	std::map<std::string,Channel>::iterator it = _channels.find(topic);
	if (it != _channels.end())	
		return(&(it->second));
	else
		return (NULL);
}

/*
return fd if found
return -1 if not found
*/
int		Server::getClientFdByUsername(std::string username)
{
	std::map<int,Client>::iterator it = _users.begin();
	std::map<int,Client>::iterator ite = _users.end();
	for(; it != ite; it++)
	{
		if ((it->second).getUsername() == username)
			return it->first;
	}
	return (-1);
}

std::string	Server::getPassword()
{
	return this->_password;
}

