/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:51:33 by aranger           #+#    #+#             */
/*   Updated: 2024/08/27 22:11:17 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Bot.hpp"

int main(int ac, char** av)
{
	if (ac != 4)
	{
		std::cerr << "Error : usage : ./bot <server ip adress> <password> <port>" << std::endl;
		return (1);	
	}
	Bot bot = Bot(av[1], av[2], av[3]);
	try 
	{
		bot.getInsults();
		bot.connectBot();
		bot.loopBot();
	}
	catch (std::exception &e)
	{
		std::cerr << "Error : " << e.what() << std::endl;
	}
}