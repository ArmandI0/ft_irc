/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboire <dboire@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:37:53 by dboire            #+#    #+#             */
/*   Updated: 2024/08/15 20:22:53 by dboire           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Messages.hpp"

// Parsing a faire avec 'sender', 'type', 'target', 'message'
int connect = 0;
int operators = 1;
std::string input;
std::string PASS = "ircpassword";

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

void server_msg(void)
{
	std::getline(std::cin, input);
	if(input == "/JOIN")
	{
		std::cout << "/JOIN" << std::endl;
	}
	if(std::strncmp(input.c_str(), "NICK", 4) == 0)
	{
		std::cout << "NICK : " << std::endl;
	}
	if(operators == 1)
	{
		if(input == "/KICK")
		{
			std::cout << "/JOIN" << std::endl;
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
	}
}

int main(void)
{
	while(1)
	{
		connection(); // The user fills his username and the password of the server
		server_msg(); // User is in the server waiting to join a room/channel
	}
}
