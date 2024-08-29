/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:51:33 by aranger           #+#    #+#             */
/*   Updated: 2024/08/28 18:41:10 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Bot.hpp"

int main(int ac, char** av)
{
	if (ac != 5)
	{
		std::cerr << "Error : usage : ./bot <server ip adress> <password> <port> <bot nickname>" << std::endl;
		return (1);	
	}
	Bot bot = Bot(av[1], av[2], av[3], av[4]);
	try 
	{
		bot.getInsults();
		bot.connectBot();
		bot.loopBot();
	}
	catch (std::exception &e)
	{
		std::cerr << "Error : " << e.what() << std::endl;
		return (1);	
	}
	return (0);
}