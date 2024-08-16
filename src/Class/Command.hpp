/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboire <dboire@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:37:51 by dboire            #+#    #+#             */
/*   Updated: 2024/08/16 09:45:14 by dboire           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Command_hpp
# define Command_hpp

# include <iostream>
# include <string>
# include <cstring>
# include <cctype>
# include <sstream>
# include <vector>
# include "Client.hpp"
# include "Server.hpp"

class Server;
class Client;
// Parsing a faire avec 'sender', 'type', 'target', 'message'

class Command
{

    public:
		Server(std::string message, Client client);
		~Server();
	private :	
		Server();
		Server(const Server& src);
		Server& operator=(const Server& src);

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