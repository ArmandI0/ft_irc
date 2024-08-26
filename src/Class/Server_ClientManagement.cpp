/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_ClientManagement.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:45:41 by aranger           #+#    #+#             */
/*   Updated: 2024/08/26 16:35:14 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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

void Server::addNewNickname(std::string & nick, Client * client)
{
	this->_nicknames.insert(std::make_pair(nick, client));
}

void	Server::addNewUsername(std::string & username, Client * client)
{
	this->_usernames.insert(std::make_pair(username, client));
}

Client&		Server::getClientByFd(int socket)
{
	Client& ref = _users[socket];
	return (ref);
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

void	Server::delClient(int client_fd)
{
	Client to_delete = getClientByFd(client_fd);
	close(client_fd);
    epoll_ctl(this->_epoll_socket, EPOLL_CTL_DEL, client_fd, NULL);
	std::map<std::string,Client*>::iterator it = this->_nicknames.find(to_delete.getNick());
	if (it != _nicknames.end())
	{
		std::cout << RED << "erase nickname:" <<  it->first << RESET << std::endl;
		this->_nicknames.erase(it);
	}
	it = this->_usernames.find(to_delete.getUsername());
	if (it != _usernames.end())
	{
		std::cout << RED << "erase username:" <<  it->first << RESET << std::endl;
		this->_usernames.erase(it);
	}
	this->_users.erase(client_fd);
	std::cout << MAGENTA << "Client : " << client_fd << " left the server." << RESET << std::endl;
}
