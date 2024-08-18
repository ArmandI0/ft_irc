/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UT_channel.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:51:33 by aranger           #+#    #+#             */
/*   Updated: 2024/08/18 14:38:07 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../src/irc.hpp"


int main(int ac, char** av)
{
	(void)ac;
	(void)av;
	Server serv("5252", "pswd");
	Client client_creator(5);
	Client client2(6);
	std::string channel_name = "topic1";
	std::string channel_name2 = "topic2";

	// create channel
	serv.createChannel(channel_name, client_creator);
	serv.createChannel(channel_name2, client_creator);
	serv.print_list_channels();

	// delete channel
/* 	serv.delChannel(channel_name);
	std::cout<< "apres delete : " << std::endl;
	serv.print_list_channels(); */
	
	// get channel by topic
	Channel& channel_test = ((serv.getChannels())[channel_name]);
	std::cout << std::endl;
	std::cout << channel_test.getChannelTopic() << std::endl;

	// change mode
	std::cout << std::endl;
	std::cout << "Mode i is now : " << channel_test.isModeOn('i') << std::endl;
	std::cout << "Mode l is now : " << channel_test.isModeOn('l') << std::endl;
	std::cout << "Change mode to +il 100" << std::endl;	
	channel_test.setUnsetUserLimit(1,100);
	channel_test.setUnsetInviteMode(1);
	std::cout << "Mode i is now : " << channel_test.isModeOn('i') << std::endl;
	std::cout << "Mode l is now : " << channel_test.isModeOn('l') << "with limit = "<< channel_test.getUserLimit() << std::endl;

	// add client
	std::cout << std::endl;
	std::cout << "Add client2 then list clients" << std::endl;	
	channel_test.addClient(client2);
	std::map<int,std::string> list_clients = channel_test.getClientsList();
	for (std::map<int,std::string>::iterator it=list_clients.begin(); it != list_clients.end();it++)
		std::cout<< "client fd, username : " << (it->first) << ":"<<it->second << std::endl;

	// del client
	std::cout << std::endl;
	std::cout << "Delete client2 then list clients" << std::endl;	
	channel_test.delClient(client2);
	list_clients = channel_test.getClientsList();
	for (std::map<int,std::string>::iterator it=list_clients.begin(); it != list_clients.end();it++)
		std::cout<< "client fd, username : " << (it->first) << ":"<<it->second << std::endl;

	
/* 	std::cout<< "--delete last client--"<<std::endl;	
	ptr->delClient(client_creator);
	ptr = serv.getChannelByTopic(channel_name);
	if (ptr != NULL)
		std::cout << ptr->getChannelModes() << std::endl;
	else
		std::cout << "channel deleted because no more client in it"<< std::endl; */
 
}