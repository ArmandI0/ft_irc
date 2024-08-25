/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:35:42 by aranger           #+#    #+#             */
/*   Updated: 2024/08/25 14:30:06 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_hpp
# define Client_hpp
# include "../irc.hpp"


class Server;
class Channel;
class Command;

class Client
{
	private :
		int		_socket;
		bool	_auth;
		bool	_pass;
	
		std::string	_nickname;
		std::string	_username;
		std::string	_second_choice;
		std::string	_third_choice;
		std::string	_entry;
		std::map<std::string, Channel*> _channels_in; // Vraiment besoin ? 
		
	public:
		Client();
		Client(int socket);
		Client(const Client& src);
		Client& operator=(const Client& src);
		~Client();
		
		int										getSocket() const;
		bool 									getAuth() const;
		bool									getPass() const;
		std::string								&getNick();
		std::string								&getUsername();
		std::string								&getEntry();
		const std::map<std::string, Channel*>	&getChannelsIn();
		
		void									setUsername(std::string username);
		void									setEntry(const std::string & str);
		void									setPass();
		void									setAuth();
		void									setNick(const std::string &str);
		void									setUser(const std::string & str);		
		void									setSocket(int socket);
		void									eraseEntry();
};

#endif