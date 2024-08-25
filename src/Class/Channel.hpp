/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboire <dboire@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 21:07:14 by dboire            #+#    #+#             */
/*   Updated: 2024/08/25 20:28:02 by dboire           ###   ########.fr       */
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
		Channel(std::string & name, Client* creator, Server* serv);
		Channel(const Channel& src);
		Channel& operator=(const Channel& src);
		~Channel();

		void					addClientToCh(Client * client);
		void					addClientToOp(Client * client);
		void					addClientToInvite(Client * client, Client * t_client);

		void					kickClient(Client* client, std::string target, std::string reason);
		void					delClientToOp(Client* client);
		void					delClient(std::string client);
		void					delChannel();

		bool					checkLimitUser();
		bool					checkInvite(std::string name);
		bool					checkIfOp(std::string name);
		
		void					notifyJoin(std::string nickname);
		void					sendMessageToAllClient(std::string error);
		void					sendMessageToAllClient(std::string sender, std::string message);

		size_t					getUserLimit();
		std::string 			getName();
		std::string				getKey();
		size_t					getLimitUser();
		bool					getInvite();
		bool					getTopicProtection();
		std::string 			getTopic();
		
		void					setKey(std::string key);
		void					setTopicMsg(std::string topic);
		void					setLimit(std::string limit);
		void					setTopic(int remove);
		void					setInvite(int remove);

		bool					hasUser(std::string nickname);
		void					printUsersInChannel(Client* client, std::string& channel_name);

	private :	
		
		std::map<std::string, Client *>	_clients;
		std::map<std::string, Client *>	_operator; // +o
		std::vector<std::string>		_invite_name;
		std::string						_name;
		Server*							_server;
		std::string						_key; // +k
		size_t							_limit_user; // +l
		bool							_invite_only; // +i
		bool							_topic_protection; // +t
		std::string						_channel_topic;
};

#endif