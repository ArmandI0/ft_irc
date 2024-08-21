/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UT_bot.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:51:33 by aranger           #+#    #+#             */
/*   Updated: 2024/08/21 12:05:09 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../bonus/Bot.hpp"


int main(int ac, char** av)
{
	(void)ac;
	(void)av;
	if (ac != 4)
	{
		std::cerr << "Error : usage : ./bot <server ip adress> <password> <port>" << std::endl;
		return (1);	
	}
	Bot bot = Bot(av[1], av[2], av[3]);
	try 
	{
		bot.connectBot();
		bot.loopBot();
	}
	catch (std::exception &e)
	{
		std::cerr << "Error : " << e.what() << std::endl;
	}
}