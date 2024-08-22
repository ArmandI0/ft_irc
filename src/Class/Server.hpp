/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 16:28:21 by aranger           #+#    #+#             */
/*   Updated: 2024/08/22 16:03:04 by aranger          ###   ########.fr       */
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
		void								execCommand(Client & client);
		std::string							getPassword();
		
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
		void								setChannel(Channel & channel, std::string& channel_name);
		void								delChannel(std::string & channel_name);
		Channel*							getChannel(std::string & channel_name);
		void								printChannels();

	private:
		Server();
		
		struct sockaddr_in					_server_infos;
		std::map<int,Client>				_users;
		std::map<std::string, Client*>		_nicknames;
		std::map<std::string, Client*>		_usernames;
		std::map<std::string, Channel>		_channels;
		std::string							_password;

		int									_epoll_socket;
		int									_listen_socket;
};

#endif