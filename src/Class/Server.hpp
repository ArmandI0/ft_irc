/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 16:28:21 by aranger           #+#    #+#             */
/*   Updated: 2024/08/20 17:36:22 by aranger          ###   ########.fr       */
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
		Server(std::string port, std::string password);
		~Server();

		/*		Server					*/
		void								listenSocket();
		void								execServer();
		std::string							readSocket(int fd);
		
		/*		Clients managements		*/
		epoll_event							addNewClient(int new_client_fd);
		void								addNewNickname(std::string & nick, Client * client);
		void								addNewUsername(std::string & username, Client * client);
		Client&								getClientByFd(int socket);
		Client*								findUserByNickname(std::string & nickname);
		Client*								findUserByUsername(std::string & username);
		void								delClient(int client_fd);
		void								delClientByNickname(std::string & nickname);
		void								delClientByUsername(std::string & username);
		
		/*		Channels managements	*/
		void								createChannel(std::string & channel_name, Client & client_creator);
		void								delChannel(std::string& channel_name);
		int									getChannelId(std::string topic);
		Channel*							getChannelByTopic(std::string topic);
		int									getClientFdByUsername(std::string username);
		std::string							getPassword();
		bool								hasChannel(std::string& channel_name);
		void								addUserToChannel(const std::string& channel_name, Client* user);
		void								setClientUsernameByFd(int socket, std::string username);
		void								print_list_channels();

	private:
		Server();
		
		struct sockaddr_in					_server_infos;
		std::map<int,Client>				_users;
		std::map<std::string, Client*>		_nicknames;
		std::map<std::string, Client*>		_usernames;
		std::map<std::string,Channel>		_channels;
		std::string							_password;

		int									_epoll_socket;
		int									_listen_socket;
};

#endif