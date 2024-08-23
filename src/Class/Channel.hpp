/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboire <dboire@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 21:07:14 by dboire            #+#    #+#             */
/*   Updated: 2024/08/23 14:28:27 by dboire           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Channel_hpp
# define Channel_hpp

# include "../irc.hpp"

class Server;
class Client;
class Command;

class Channel
{
	public:
		Channel();
		Channel(const std::string& name, Client* creator, Server* serv);
		Channel(const Channel& src);
		Channel& operator=(const Channel& src);
		~Channel();
		
		void					addClientToCh(Client * client);
		void					addClientToOp(Client * client);
		void					delClient(std::string nickname);
		void					delChannel();
		
		void					notifyJoin(std::string nickname);

		std::string					getPassword();
		size_t						getUserLimit();
		std::string 				getName();

		bool					isModeOn(char mode);
		bool					isOperator(int socket_user);
		
		bool					hasUser(std::string nickname);

		void 					setUnsetTopicRestr(bool on_off);
		void 					setUnsetInviteMode(bool on_off);
		void 					setUnsetPassword(bool on_off, std::string password);
		void 					setUnsetUserLimit(bool on_off, size_t user_limit);
		void					setUnsetOpPrivilege(bool on_off, std::string username);
		void					setTopicName(std::string topic_name);
		void					setName(std::string name);
		void					printUsersInChannel(Client* client, std::string& channel_name);

	private :	
		void					removeOperatorPrivilege(std::string username);
		void					addOperatorPrivilege(std::string username);
		
		std::map<std::string, Client *>	_clients;
		std::map<std::string, Client *>	_operator; // +o
		std::string						_name;
		Server*							_server;
		std::string						_key; // +k
		size_t							_limit_user; // +l
		bool							_invite_only; // +i
		bool							_topic_protection; // +t
		std::string						_channel_topic;
		
		
		
		// To delete
		std::map<int, std::string>	_operators;
};

#endif