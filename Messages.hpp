/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboire <dboire@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:37:51 by dboire            #+#    #+#             */
/*   Updated: 2024/08/16 09:45:14 by dboire           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Messages_hpp
# define Messages_hpp

#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <sstream>
#include <vector>
// Parsing a faire avec 'sender', 'type', 'target', 'message'

class Command
{
	std::string command;
	int	command_type; // KICK, INVITE, TOPIC, MODE with sub modes
};


class Messages
{
	int	msg_to_serv;
	int msg_to_client;

	std::string sender;
	int	sender_socket;
	
	std::string type;
	int	command_type;
	
	std::string target;
	int	target_socket;
	
	std::string message;
};


#endif