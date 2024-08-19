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

// int 	Command::parsing()
// {
// 	return (0);
// }

// int 	Command::exec()
// {
// 	int r_value = parsing();
// 	return (r_value);
// }

// void	Command::response(std::string str_response, std::vector<Client> clients)
// {
// 	(void)str_response;
// 	(void)clients;
// }


//Parsing a faire avec 'sender', 'type', 'target', 'message'
int connected = 0;
int operators = 1;
std::string input;

void nickname_command(std::vector<Channel> channels)
{
	(void)channels;
	std::string::iterator it = input.begin();
	if (input.compare(0, 5, "NICK ") == 0)
	{
		// if(!nickname.empty())
		// {
		// 	std::cout << "You already have a nickname, to change please enter :<Nickname> NICK <New Nickname>" << std::endl;
		// 	return ;
		// }
		it += 5;
		while (it != input.end() && std::isspace(*it))
			++it;
		std::string::iterator start = it;
		while (it != input.end() && std::isalpha(*it))
			++it;
		std::string temp_nickname(start, it);
		while (it != input.end())
		{
			if (!std::isspace(*it))
			{
				std::cout << "Error in the nickname command" << std::endl;
				return;
			}
			++it;
		}
		// nickname = temp_nickname.c_str();
		// std::cout << "Introducing new nick " << nickname << std::endl;
	}
}

// void nickname_change(std::vector<Channel> channels)
// {
// 	std::string::iterator it = input.begin();
// 	if (input.compare(1, nickname.size(), nickname) == 0)
// 	{
// 		it += nickname.size();
// 		while (it != input.end() && std::isspace(*it))
// 			++it;
// 		if (input.compare(nickname.size() + 1, 6, " NICK ") == 0)
// 			it += 7;
// 		std::string::iterator start = it;
// 		while (it != input.end() && std::isalpha(*it))
// 			++it;
// 		std::string temp_nickname(start, it);
// 		while (it != input.end())
// 		{
// 			if (!std::isspace(*it))
// 			{
// 				std::cout << "Error in the nickname command" << std::endl;
// 				return;
// 			}
// 			++it;
// 		}
// 		nickname = temp_nickname.c_str();
// 		std::cout << "Introducing new nick " << nickname << std::endl;
// 	}
// }

// void kick_command(std::vector<Channel> channels)
// {
// 	std::string::iterator it = input.begin();
// 	it += 6;
// 	while (it != input.end() && std::isspace(*it))
// 		++it;
// 	std::string::iterator start = it;
// 	if(it == input.end())
// 	{
// 		std::cout << "Please enter the name of the user you want to kick" << std::endl;
// 		return ;
// 	}
// 	while (it != input.end() && std::isalpha(*it))
// 		++it;
// 	std::string temp_name(start, it);
// 	while (it != input.end())
// 	{
// 		if (!std::isspace(*it))
// 		{
// 			std::cout << "Error in the nickname command" << std::endl;
// 			return;
// 		}
// 		++it;
// 	}
// 	std::cout << "Kicking : " << temp_name << std::endl;
// }

// Submode

void i_submode(std::string input, std::vector<Channel> channels)
{
	std::string::iterator it = input.begin();
	it += 9;
	while (it != input.end() && std::isspace(*it))
		++it;
	std::string::iterator start = it;
	while (it != input.end() && (std::isalpha(*it) || std::isdigit(*it)))
		++it;
	while (it != input.end() && std::isspace(*it))
		++it;
	if(it == input.end())
	{
		std::string temp_name(start, it);
		for(auto &ch : channels)
		{
			if(temp_name == ch.getName())
			{
				std::cout << "Channel : " << ch.getName() << std::endl;
			}
		}
	}
}

void t_submode(std::string input, std::vector<Channel> channels)
{
	std::string::iterator it = input.begin();
	it += 9;
	while (it != input.end() && std::isspace(*it))
		++it;
	std::string::iterator start = it;
	while (it != input.end() && (std::isalpha(*it) || std::isdigit(*it)))
		++it;
	while (it != input.end() && std::isspace(*it))
		++it;
	if(it == input.end())
	{
		std::string temp_name(start, it);
		for(auto &ch : channels)
		{
			if(temp_name == ch.getName())
			{
				std::cout << "Channel : " << ch.getName() << std::endl;
			}
		}
	}
}

void k_submode(std::string input, std::vector<Channel> channels)
{
	std::string::iterator it = input.begin();
	it += 9;
	while (it != input.end() && std::isspace(*it))
		++it;
	std::string::iterator start = it;
	while (it != input.end() && (std::isalpha(*it) || std::isdigit(*it)))
		++it;
	while (it != input.end() && std::isspace(*it))
		++it;
	if(it == input.end())
	{
		std::string temp_name(start, it);
	for(auto &ch : channels)
	{
		if(temp_name == ch.getName())
		{
			std::cout << "Channel : " << ch.getName() << std::endl;
		}
	}
	}
}

void mode_command(std::vector<Channel> channels)
{
	std::string::iterator it = input.begin();
	it += 6;
	if(*it != '#')
	{
		std::cout << "Here are the options for mode : +i, +t, +k, +o, +l" << std::endl;
		return ;
	}
	it++;
	std::string::iterator start = it;
	while (it != input.end() && (std::isalpha(*it) || std::isdigit(*it)))
		++it;
	std::string temp_name(start, it);
	for(auto &ch : channels)
	{
		if(temp_name == ch.getName())
		{
			std::cout << "Channel : " << ch.getName() << std::endl;
		}
	}
	
	if (input.compare(6, 1, "+") == 0)
	{
		if (input.compare(7, 2, "i ") == 0)
			i_submode(input, channels);
		if (input.compare(7, 2, "t ") == 0)
			t_submode(input, channels);
		if (input.compare(7, 2, "k ") == 0)
			k_submode(input, channels);
		if (input.compare(7, 2, "o ") == 0)
			//o_submode(it + 2);
		if (input.compare(7, 2, "l ") == 0)
		{}//l_submode(it + 2);
	}
	else
		std::cout << "Please put a + after /MODE" << std::endl;
}

void error_message(std::string error)
{
	std::cout << "Syntax error : " << error << std::endl;
}
	
	
void join_command()
{
	std::cout << "You are in the join command" << std::endl;
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
