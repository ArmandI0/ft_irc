/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 16:28:21 by aranger           #+#    #+#             */
/*   Updated: 2024/08/13 21:19:10 by aranger          ###   ########.fr       */
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


# include "Client.hpp"

//# include "Channel.hpp"


class Server
{
    private:
		struct sockaddr_in		_server_infos; //ip_type ; adr server ; port
		std::map<int,Client>	_users;  //store int_fd for each client
		//std::vector<Channel>	_channels;
		std::string				_password;

		int						_epoll_socket;
		int						_listen_socket;
      
		Server(/* args */);

    public:
		Server(std::string port, std::string password); // create listen socket
		~Server();

		void	listenSocket();
		void	execServer(); // lancer epoll



		void	addClient(); // ajoute a std::map user
		void	addChannel();      
};

#endif