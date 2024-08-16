/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 16:28:21 by aranger           #+#    #+#             */
/*   Updated: 2024/08/16 12:49:40 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Server_hpp
# define Server_hpp
# include <vector>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <netdb.h>
# include <map>
# include <vector>
# include <cstdlib>

# include "Client.hpp"
# include "Command.hpp"
# include "Channel.hpp"

//# include "Channel.hpp"

class Command;
class Channel;
class Client;

class Server
{
    private:
		struct sockaddr_in				_server_infos; //ip_type ; adr server ; port
		std::map<int,Client>			_users;  //store int_fd for each client
		std::map<std::string,Client>	_channels;
		std::string						_password;

		int								_epoll_socket;
		int								_listen_socket;
      
		Server(/* args */);

    public:
		Server(std::string port, std::string password); // create listen socket
		~Server();

		void		listenSocket();
		void		execServer(); // lancer epoll


		void		clientAuth();
		epoll_event	addClient(int new_client_fd); // ajoute a std::map user
		void		addChannel();      
};

#endif