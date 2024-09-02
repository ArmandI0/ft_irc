/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:51:33 by aranger           #+#    #+#             */
/*   Updated: 2024/09/02 10:39:50 by nledent          ###   ########.fr       */
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
	for (int i=1;i<5;i++)
	{
		std::string arg = av[i];
		if (arg.empty())
		{
			std::cerr << "Error : please provide non null parameters" << std::endl;
			return (1);	
		}
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