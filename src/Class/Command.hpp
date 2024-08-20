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
		Command(std::string& input, Client* client, Server* server);
		Command(const Command& src);
		Command& operator=(const Command& src);
		~Command();

		// void		exec_pass();
		void		execJoin();
		void		execNick();
		// int		parsing();
		// int		exec();
		void		server_msg();

		/* COMMANDE D'AUTHENTIFICATION */
		int			serverAuth();
		void		doCommandAuth(std::string cmd);
		int			passCommand(std::vector<std::string> & password);
		void		userCommand(std::vector<std::string> & username);
		int			nickCommand(std::vector<std::string> & nickname);

		// void	response(std::string str_response, std::vector<Client> clients);
	private :	
		std::string					_input;
		Client*						_client_requester;
		Server*						_server;
		std::vector<std::string>	_command;
};

bool	checkAndComp(std::vector<std::string> & entry, size_t i, const char* toComp);
void	sendErrorToClient(int fd, std::string error);

#endif