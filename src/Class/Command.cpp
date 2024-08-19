/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboire <dboire@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:37:53 by dboire            #+#    #+#             */
/*   Updated: 2024/08/16 10:03:38 by dboire           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Command::Command(std::string& input, Client* client): _input(input), _client_requester(client)
{
}
Command::Command(std::string& input, Client* client, Server* server) : _input(input), _client_requester(client), _server(server)
{

}

Command::Command()
{
}

Command::Command(const Command& src)
{
	*this = src;
}

Command& Command::operator=(const Command& src)
{
	this->_client_requester = src._client_requester;
	this->_input = src._input; 
	return(*this);
}

Command::~Command()
{
}


void error_message(std::string error)
{
	std::cout << "Syntax error : " << error << std::endl;
}
	

void	Command::execJoin()
{
	if(this->_server->hasChannel(this->_command[1]) == true)
	{
		this->_server->addUserToChannel(this->_command[1], _client_requester);
	}
}

void	Command::execNick()
{
	std::string authorized_first_character = "-^~";
	std::string invalid_chars = ",:!@.";
	if(!isalpha(this->_command[1][0]) || (authorized_first_character.find(this->_command[1][0]) == std::string::npos))
	{
		std::cout << "First letter of the Nick must be a letter or - ^ ~" << std::endl;
		return ;
	}
	for(char c : this->_command[1])
	{
		if(invalid_chars.find(c) != std::string::npos)
		{
			std::cout << "You have placed a forbidden character '" << c << "' in your Nickname" << std::endl;
			return ;
		}
	}
	//4. Nickname Change Notification TO DO
		//Description: When a user changes their nickname, the IRC server broadcasts this change to all other users in the channels where the user is present.
	this->_client_requester->setNick(this->_command[1]);
}

void Command::server_msg()
{

	std::istringstream iss(this->_input);
	std::string content;

	while (std::getline(iss, content))
		this->_command.push_back(content);

	if (this->_command[0] == "/QUIT" && this->_command[1].empty())
		; // function to exit the irc server and free the socket

	if (this->_command[0] == "/JOIN" && !this->_command[1].empty() && this->_command[2].empty())
		execJoin();

	if (this->_command[0] == "NICK" && !this->_command[1].empty() && this->_command[2].empty()) // OK
		execNick();
	// // if(input[0] == ':')
	// // 	nickname_change(channels);
	// if(operators == 1)
	// {
	// 	// if (input.compare(0, 6, "/KICK ") == 0)
	// 	// {
	// 	// 	kick_command(channels);
	// 	// }
	// 	if(command == "/INVITE")
	// 	{
	// 		std::cout << "/INVITE" << std::endl;
	// 	}
	// 	if(command == "/TOPIC")
	// 	{
	// 		std::cout << "/TOPIC" << std::endl;
	// 	}
	// 	if (command == "MODE ") // Need to be exactly like : MODE #channel +mode target_user / Example :Carol!carol@irc.example.com MODE #chatroom +v Dave
	// 	{
	// 		mode_command(channels);
	// 		//go in function to see sub div mode
	// 	}
	// 	if(input == "EXIT")
	// 		exit(0);
	// }

}

int	Command::serverAuth()
{
	std::istringstream iss(this->_input);
	std::vector<std::string>	command;
	std::string content;
	std::string rest;
	while (std::getline(iss, content))
	{
		command.push_back(content);
	}
	this->_input = content;
	if (command[0] == "PASS" && this->_client_requester->getPass() == false && rest.empty() == true)
	{
		this->passCommand();
	}
	else if (command[0] == "NICK" && rest.empty() == true)
	{
		this->nickCommand();
	}
	else
	{
		std::cout << "Enter a valid command PASS / NICK" << std::endl;
	}
	if (this->_client_requester->getPass() == true && this->_client_requester->getNick().empty() == false)
		this->_client_requester->setAuth();
	return(0);
}

int	Command::passCommand()
{
	if(this->_input.compare(this->_server->getPassword()) == 0)
	{
		(this->_client_requester)->setPass();
		return 1;
	}
	return 0;
}

int	Command::nickCommand()
{
	if (this->_input.empty())
	{
		std::cerr << "Error : Invalid Nickame" << std::endl;
	}
	else
		(this->_client_requester)->setNick(this->_input);
	return (1);
}
