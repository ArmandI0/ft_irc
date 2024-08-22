/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboire <dboire@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 21:07:14 by dboire            #+#    #+#             */
/*   Updated: 2024/08/22 21:42:01 by dboire           ###   ########.fr       */
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
		
		void					addClientToCh(Client* client);
		void					delClient(std::string nickname);
		void					delChannel();

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

	private :	
		void					removeOperatorPrivilege(std::string username);
		void					addOperatorPrivilege(std::string username);
		
		std::map<std::string, Client *>	_clients;
		std::map<int, std::string>	_operators;
		std::string				_channel_topic;
		std::string				_name;
		Server*					_server;
		std::string				_password;
		size_t					_user_limit;
		bool					_invite_mode;
		bool					_topic_op_mode;
};

#endif