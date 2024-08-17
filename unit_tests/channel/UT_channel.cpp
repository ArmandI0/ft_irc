/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UT_channel.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:51:33 by aranger           #+#    #+#             */
/*   Updated: 2024/08/17 23:18:53 by nledent          ###   ########.fr       */
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

	// create channel
	serv.createChannel(channel_name, client_creator);
	serv.print_list_channels();

	// delete channel
/* 	serv.delChannel(channel_name);
	std::cout<< "apres delete : " << std::endl;
	serv.print_list_channels(); */
	
	// get channel by topic
	Channel channel_test = ((serv.getChannels())[channel_name]);
	std::cout << channel_test.getChannelTopic() << std::endl;

	// change mode
	std::string str_mode = "i+k+l+o-";
	((serv.getChannels())[channel_name]).changeChannelMode(str_mode);
	Channel *ptr = serv.getChannelByTopic(channel_name);
	if (ptr != NULL)
		std::cout << ptr->getChannelModes() << std::endl;
	
	str_mode = "i-k+o+";
	((serv.getChannels())[channel_name]).changeChannelMode(str_mode);
	ptr = serv.getChannelByTopic(channel_name);
	if (ptr != NULL)
		std::cout << ptr->getChannelModes() << std::endl;

	// check if mode is on
	if (ptr->isModeOn('i') == true)
		std::cout << "mode i : yes" << std::endl;
	else
		std::cout << "mode i : no" << std::endl;

	if (ptr->isModeOn('o') == true)
		std::cout << "mode o : yes" << std::endl;
	else
		std::cout << "mode o : no" << std::endl;

	// add client
	ptr->addClient(client2);
	std::map<int,Client*> list_clients = ptr->getClients();
	for (std::map<int,Client*>::iterator it=list_clients.begin(); it != list_clients.end();it++)
		std::cout<< "client fd : " << (it->second)->getSocket() << std::endl;

	// del client
	std::cout<< "--delete client--"<<std::endl;	
	ptr->delClient(client2);
	list_clients = ptr->getClients();
	for (std::map<int,Client*>::iterator it=list_clients.begin(); it != list_clients.end();it++)
		std::cout<< "client fd : " << (it->second)->getSocket() << std::endl;	

	std::cout<< "--delete last client--"<<std::endl;	
	ptr->delClient(client_creator);
	ptr = serv.getChannelByTopic(channel_name);
	if (ptr != NULL)
		std::cout << ptr->getChannelModes() << std::endl;
	else
		std::cout << "channel deleted because no more client in it"<< std::endl;

}