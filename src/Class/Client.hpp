/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:35:42 by aranger           #+#    #+#             */
/*   Updated: 2024/08/14 16:26:51 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_hpp
# define Client_hpp


class Client
{
	private :
		int	_socket;

			
		//grade;
		std::string	_nickname;
		std::string	_username;
		std::string	_second_choice;
		std::string	_third_choice;

	public:
		Client();
		~Client();
  
};

#endif