/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command_Auth.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:22:46 by aranger           #+#    #+#             */
/*   Updated: 2024/08/26 16:30:49 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::passCommand(std::vector<std::string> & password)
{
	if (this->_client_requester->getPass() == true)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_ALREADYREGIST);
	else if (password.size() == 1)
	{
		sendMessageToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS(this->_client_requester->getNick() ,"PASS"));
	}
	else if(password[1].compare(this->_server->getPassword()) == 0 && password.size() == 2)
	{
		(this->_client_requester)->setPass();
        std::cout << MAGENTA << "exec PASS :" << password[1] << RESET << std::endl;
	}
}

void	Command::nickCommand(std::vector<std::string> & nickname)
{
	if (nickname.size() == 1)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_NONICKNAMEGIVEN);
	else if (nickname.size() == 2 && nickname[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_") != std::string::npos)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_ERRONEUSNICKNAME(nickname[1]));	
	else if (nickname.size() == 2 && this->_server->findUserByNickname(nickname[1]) != NULL)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_NICKNAMEINUSE(nickname[1]));
	else if (nickname.size() == 2)
	{
		this->_client_requester->setNick(nickname[1]);
		this->_server->addNewNickname(nickname[1], _client_requester);
        std::cout << MAGENTA << "exec NICK :" << nickname[1] << RESET << std::endl;
	}
}

void Command::userCommand(std::vector<std::string> & username)
{
	if (username.size() < 5)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS(this->_client_requester->getNick(), "USER"));
	else if (username.size() == 5 && this->_server->findUserByUsername(username[1]) != NULL)
	{
		std::cout << "borldel" << std::endl;	
		sendMessageToClient(this->_client_requester->getSocket(), ERR_ALREADYREGIST);
	}
	else if (username.size() == 5)
	{
		this->_client_requester->setUser(username[1]);
		this->_server->addNewUsername(username[1], _client_requester);
        std::cout << MAGENTA << "exec USER :" << username[1] << RESET << std::endl;
	}
}