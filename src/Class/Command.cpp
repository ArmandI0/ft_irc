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
	// std::cout << "String 2:" << this->_input.substr(1) << std::endl;
	// std::cout << "exiting" << std::endl;
void join_command(std::vector<Channel> channels, std::vector<Client> clients)
{
	(void)clients;
	std::string::iterator it = input.begin();
	it += 6;
	if(it == input.end())
		error_message("Nothing after /JOIN");
	it++;
	if(*it != '#')
		error_message("Enter a #channel after /JOIN");
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
}

void Command::server_msg()
{
	std::istringstream iss(this->_input);
	std::string split_string;
	std::string split_string2;
	
	iss >> split_string;
	iss >> split_string2;
	// std::cout << "Split Word : "<< split_string << std::endl;
	this->_input.erase(0, split_string.length() + 1);
	// std::cout << "String :" << this->_input << std::endl;
	// std::cout << "Split Word 2:"<< split_string2 << std::endl;
	this->_input.erase(0, split_string2.length());
	// std::cout << "String 2:" << this->_input.substr(1) << std::endl;
	// std::cout << "exiting" << std::endl;


	// if (split_string == "/JOIN")
	// 	join_command();
	// if (split_string == "NICK")
	// 	nickname_command(channels);
	// // if(input[0] == ':')
	// // 	nickname_change(channels);
	// if(operators == 1)
	// {
	// 	// if (input.compare(0, 6, "/KICK ") == 0)
	// 	// {
	// 	// 	kick_command(channels);
	// 	// }
	// 	if(split_string == "/INVITE")
	// 	{
	// 		std::cout << "/INVITE" << std::endl;
	// 	}
	// 	if(split_string == "/TOPIC")
	// 	{
	// 		std::cout << "/TOPIC" << std::endl;
	// 	}
	// 	if (split_string == "MODE ") // Need to be exactly like : MODE #channel +mode target_user / Example :Carol!carol@irc.example.com MODE #chatroom +v Dave
	// 	{
	// 		mode_command(channels);
	// 		//go in function to see sub div mode
	// 	}
	// 	if(input == "EXIT")
	// 		exit(0);
	// }

}

 