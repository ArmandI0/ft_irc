/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboire <dboire@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 16:28:21 by aranger           #+#    #+#             */
/*   Updated: 2024/08/21 14:22:04 by dboire           ###   ########.fr       */
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
		

		void								delChannel(std::string& channel_name);
		int									getChannelId(std::string topic);
		// Channel*							getChannelByTopic(std::string topic);
		int									getClientFdByUsername(std::string username);
		std::string							getPassword();
		Channel*							hasChannel(std::string& channel_name);
		void								addUserToChannel(const std::string& channel_name, Client* user);
		Client&								getClientByFd(int socket);
		void								setClientUsernameByFd(int socket, std::string username);
		void								print_list_channels();
		
		void								setChannel(Channel channel, std::string& channel_name);

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