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

Command::Command(const std::string& input, Client& client): _input(input), _client_requester(&client)
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
	_client_requester = src._client_requester;
	return(*this);
}

Command::~Command()
{
}

int 	Command::parsing()
{
	return (0);
}

int 	Command::exec()
{
	int r_value = parsing();
	return (r_value);
}

void	Command::response(std::string str_response, std::vector<Client> clients)
{
	(void)str_response;
	(void)clients;
}
// Parsing a faire avec 'sender', 'type', 'target', 'message'
/* int connect = 0;
int operators = 1;
std::string input;
std::string PASS = "ircpassword";
const char* nickname[10];
int i = 0; */

/* std::vector<std::string> split(const std::string &str, const char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);
	
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return (tokens);
}

void connection(void)
{
		std::cout << "User :";
		std::getline(std::cin, input);
		//define user
		std::cout << "PASS :";
		std::getline(std::cin, input);
		if(input != PASS)
		{
			std::cout << std::endl;
			std::cout << "Wrong password, try again" << std::endl;
			connection();
		}
}

void nickname_command(const char* input_char)
{
	const char* temp_nickname = input_char + 5;
	if(std::strncmp(input_char, "NICK ", 5) == 0)
	{
		std::cout << "NICK : " << std::endl;
		if(nickname != NULL)
		{
			std::cout << "You already have a nickname, to change please enter :<Nickname> NICK <New Nickname>" << std::endl;
			return ;
		}
		input_char += 4;
		while(*input_char && std::isspace(*input_char))
			input_char++;
		while(*input_char && std::isalpha(*input_char))
			input_char++;
		while(*input_char)
		{
			if (!std::isspace(*input_char))
			{
				std::cout << "Error in the nickname command" << std::endl;
				return ;
			}
			else
			{
				input_char++;
			}
		}
		nickname[i] = temp_nickname;
		i++;
		if(i == 11)
			i = 0;
		std::cout << "Introducing new nick " << nickname << std::endl;
	}
}

void server_msg(void)
{
	const char*	input_char = input.c_str();
	
	std::getline(std::cin, input);
	if(input == "/JOIN")
	{
		std::cout << "/JOIN" << std::endl;
	}
	nickname_command(input_char);
	if(operators == 1)
	{
		if(std::strncmp(input_char, "/KICK ", 6) == 0)
		{
			std::vector<std::string> name;
			name = split(input, ' ');
			if(name.size() > 2)
			{
				std::cout << "Error in the kicking command" << std::endl;
				return ;
			}
			std::cout << "Kicking : " << name[1] << std::endl;
		}
		if(input == "/INVITE")
		{
			std::cout << "/INVITE" << std::endl;
		}
		if(input == "/TOPIC")
		{
			std::cout << "/TOPIC" << std::endl;
		}
		if(input == "/MODE")
		{
			std::cout << "/MODE" << std::endl;
			//go in function to see sub div mode
		}
		if(input == "EXIT")
			exit(0);
	}
}

int main(void)
{
	connection(); // The user fills his username and the password of the server
	while(1)
	{
		server_msg(); // User is in the server waiting to join a room/channel
	}
}
 */