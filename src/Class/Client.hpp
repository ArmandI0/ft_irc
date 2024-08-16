/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:35:42 by aranger           #+#    #+#             */
/*   Updated: 2024/08/16 12:58:40 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_hpp
# define Client_hpp
# include <string>

class Client
{
	private :
		int	_socket;

		std::string	_nickname;
		std::string	_username;
		std::string	_second_choice;
		std::string	_third_choice;
		Client();
		
		//<std::string channel_name, Channel& channel>
		std::map<std::string, Channel&> _channels_in; 
	public:
		Client(int socket);
		~Client();
};

#endif