/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:35:42 by aranger           #+#    #+#             */
/*   Updated: 2024/08/16 14:02:39 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_hpp
# define Client_hpp
# include <string>

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
		
		//<std::string channel_name, Channel& channel>
	public:
		Client();
		Client(int socket);
		~Client();
		
		int		getSocket() const;
		bool 	getAuth() const;
		bool	getPass() const;
		bool	getNick() const;
};

#endif