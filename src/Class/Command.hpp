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

# include "../irc.hpp"

class Server;
class Channel;
class Client;
// Parsing a faire avec 'sender', 'type', 'target', 'message'

class Command
{

	public:
		Command();
		Command(std::string& input, Client* client);
		Command(const Command& src);
		Command& operator=(const Command& src);
		~Command();
		void		exec_pass();
		void		exec_join();
		// int		parsing();
		// int		exec();
		void		server_msg();

		// void	response(std::string str_response, std::vector<Client> clients);
	private :	
		std::string		_input;
		Client*			_client_requester;		
};


#endif