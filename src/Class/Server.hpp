/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 16:28:21 by aranger           #+#    #+#             */
/*   Updated: 2024/08/20 16:19:07 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Server_hpp
# define Server_hpp
# include "../irc.hpp"

class Client;
class Command;
class Channel;

class Server
{
    public:
		Server(std::string port, std::string password); // create listen socket
		~Server();

		void								listenSocket();
		void								execServer(); // lancer epoll
		std::string							readSocket(int fd);


		void								clientAuth(Client & client, Command & cmd);
		epoll_event							addClient(int new_client_fd);
		void								delClient(int client_fd);
		
		void								addNewNickname(std::string & nick, Client * client);
		Client*								findUserByNickname(std::string & nickname);
		void								addNewUsername(std::string & username, Client * client);
		Client*								findUserByUsername(std::string & username);
		
		void								createChannel(std::string & channel_name, Client & client_creator);
		void								delChannel(std::string& channel_name);
		std::map<std::string,Channel>&		getChannels();
		Channel*							getChannelByTopic(std::string topic);
		int									getClientFdByUsername(std::string username);
		std::string							getPassword();
		void								print_list_channels();
		bool								hasChannel(std::string& channel_name);
		void								addUserToChannel(const std::string& channel_name, Client* user);
		
		
	private:
		Server();
		struct sockaddr_in				_server_infos; //ip_type ; adr server ; port
		std::map<int,Client>			_users;  //store int_fd for each client
		std::map<std::string, Client*>	_nicknames; // store nickname and client
		std::map<std::string, Client*>	_usernames; 
		std::map<std::string,Channel>	_channels;
		std::string						_password;

		int								_epoll_socket;
		int								_listen_socket;
};

#endif