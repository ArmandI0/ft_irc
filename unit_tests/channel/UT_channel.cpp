/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UT_channel.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:51:33 by aranger           #+#    #+#             */
/*   Updated: 2024/08/16 16:35:12 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../src/irc.hpp"


int main(int ac, char** av)
{
	(void)ac;
	(void)av;
	Server serv("5252", "pswd");
	Client client_creator(5);
	std::string channel_name = "topic1";
	serv.createChannel(channel_name, client_creator);
	serv.print_list_channels();
	serv.delChannel(channel_name);
	std::cout<< "apres delete : " << std::endl;
	serv.print_list_channels();
}