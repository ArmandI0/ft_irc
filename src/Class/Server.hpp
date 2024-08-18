/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 16:28:21 by aranger           #+#    #+#             */
/*   Updated: 2024/08/18 13:44:49 by nledent          ###   ########.fr       */
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


		void								clientAuth();
		epoll_event							addClient(int new_client_fd); // ajoute a std::map user
		void								createChannel(std::string& channel_name, Client& client_creator);
		void								delChannel(std::string& channel_name);
		std::map<std::string,Channel>&		getChannels();
		Channel*							getChannelByTopic(std::string topic);
		int									getClientFdByUsername(std::string username);
		void								print_list_channels();
		
    private:
		Server();
		struct sockaddr_in				_server_infos; //ip_type ; adr server ; port
		std::map<int,Client>			_users;  //store int_fd for each client
		std::map<std::string,Channel>	_channels;
		std::string						_password;

		int								_epoll_socket;
		int								_listen_socket;
};

#endif