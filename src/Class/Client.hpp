/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:35:42 by aranger           #+#    #+#             */
/*   Updated: 2024/08/18 17:59:51 by nledent          ###   ########.fr       */
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
		int	_socket;

		bool	_auth;
		bool	_pass;
		bool	_nick;
	
		std::string	_nickname;
		std::string	_username;
		std::string	_second_choice;
		std::string	_third_choice;
		std::map<std::string, Channel*> _channels_in;
		
	public:
		Client();
		Client(int socket);
		Client(const Client& src);
		Client& operator=(const Client& src);
		~Client();
		
		int			getSocket() const;
		bool 		getAuth() const;
		bool		getPass() const;
		bool		getNick() const;
		std::string getUsername() const;
		void		setUsername(std::string username);
		
		void		setNick(const std::string &str);
		void		setSocket(int socket);

};

#endif