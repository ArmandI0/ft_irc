/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 16:28:21 by aranger           #+#    #+#             */
/*   Updated: 2024/08/20 17:48:08 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Bot_hpp
# define Bot_hpp
# include <iostream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <netdb.h>
# include <cstdlib>
# include <map>
# include <vector>
# include <list>
# include <unistd.h>
# include <sstream>
# include <string.h>
# define PORT 4243
# define BOT "Best_Bot"

class Bot
{
    public:
		Bot(char* ip, char *pass, char *port);
		Bot(const Bot& src);
		Bot& operator=(const Bot& src);
		~Bot();

		void				connectBot();
		void				loopBot();
		
	private:
		void				waitingEvents();
		std::string 		readSocket(int fd);
		void				authentification();
		void				createBotChannel();
		void				botResponse(std::string& request);


		struct sockaddr_in	_bot_infos;
		std::string			_nickname;
		std::string			_username;
		int					_server_socket;
		char*				_server_ip;
		std::string			_server_pass;
		char*				_server_port;
		int					_epoll_socket;
		struct addrinfo*	_res;
		Bot();
};

#endif