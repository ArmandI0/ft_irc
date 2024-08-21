/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:48:29 by aranger           #+#    #+#             */
/*   Updated: 2024/08/21 17:15:23 by aranger          ###   ########.fr       */
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
	
}

void    Server::listenSocket()
{
	int status;
	int log = 10;

	this->_listen_socket = socket(this->_server_infos.sin_family, SOCK_STREAM, 0);
	if (this->_listen_socket == -1)
	{
		std::cerr << "Error TBD"<< std::endl;
		return ;
	}    
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

epoll_event	Server::addNewClient(int new_client_fd)
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
		std::cout << client.getSocket() << "Buffer read" << buffer << std::endl;
		if (!buffer.empty())
		{
			client.setEntry(buffer);
			std::cout << "Buffer client" << client.getEntry() << std::endl;
			if (client.getEntry().find("\n") != std::string::npos)
			{
				Command	new_command(client.getEntry(), &client, &*this);
				client.eraseEntry();
				std::cout << "buffer '" << buffer << "' buffer" << std::endl;
				if(client.getAuth() == false)
					new_command.serverAuth();
				else
					new_command.server_msg();
			}
			std::cout << "Buffer client" << client.getEntry() << std::endl;
		}
	}
}

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
    else // bytes_read == -1
    {
        std::cerr << "Erreur lors de la réception de données du fd " << fd << std::endl;
        return "";
    }
}

Channel*	Server::hasChannel(std::string& channel_name)
{
	std::map<std::string,Channel>::iterator it  = this->_channels.find(channel_name);
	if(it != _channels.end())
		return (&it->second);
	std::cout << "The channel : " << channel_name << " does not exist" << std::endl;
	return(NULL);
}

void	Server::addUserToChannel(const std::string& channel_name, Client* user)
{
	std::map<std::string, Channel>::iterator it = this->_channels.find(channel_name);
	if(it != _channels.end())
	{
		it->second.addNewClient(user);
		std::cout << "Welcome : " << user->getNick() << "!" << std::endl; 
	}
	else	
	{
		std::cout << "Channel does not exist" << std::endl;
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


void	Server::setClientUsernameByFd(int socket, std::string username)
{
	_users[socket].setUsername(username);
}

Client&		Server::getClientByFd(int socket)
{
	Client& ref = _users[socket];
	return (ref);
}

void	Server::delChannel(std::string& channel_name)
{
	std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
	if(it != _channels.end())
	_channels.erase(it);
}

void	Server::print_list_channels()
{
	std::map<std::string, Channel>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); it++)
		std::cout << it->first << std::endl;
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
// Channel*	Server::getChannelByTopic(std::string topic)
// {
// 	std::vector<Channel>::iterator it = _channels.begin();
// 	std::vector<Channel>::iterator ite = _channels.end();
// 	for(; it != ite; it++)
// 	{
// 		if (it->getChannelTopic() == topic)
// 			return (&(*it));
// 	}
// 	return (NULL);
// }

void Server::setChannel(Channel channel, std::string& channel_name)
{
	_channels[channel_name] = channel;
}