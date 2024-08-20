/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UT_channel.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aranger <aranger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:51:33 by aranger           #+#    #+#             */
/*   Updated: 2024/08/20 17:21:27 by aranger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../src/irc.hpp"


int main(int ac, char** av)
{
	(void)ac;
	(void)av;
	Server serv("5252", "pswd");
	serv.addNewClient(5);
	serv.addNewClient(6);
	serv.setClientUsernameByFd(5, "El matador");
	serv.setClientUsernameByFd(6, "User lambda");
	Client& client_creator = serv.getClientByFd(5);
	Client& client2 = serv.getClientByFd(6);
	std::string channel_name = "topic1";
	std::string new_channel_name = "NEWtopic1";
	std::string channel_name2 = "topic2";

	// create channel
	std::cout << "Create two channels then print the list of channels :" << std::endl;	
	serv.createChannel(channel_name, client_creator);
	serv.createChannel(channel_name2, client_creator);
	serv.print_list_channels();
	
	// get channel by topic
	std::cout << std::endl;
	std::cout << "Get channel by topic '"<< channel_name <<"' :" << std::endl;		
	Channel* channel_test = serv.getChannelByTopic(channel_name);
	std::cout << channel_test->getChannelTopic() << std::endl;

	// change channel topic
	std::cout << std::endl;
	std::cout << "Change channel topic '"<< channel_name <<"' to : " << new_channel_name << std::endl;	
	channel_test->setTopicName(new_channel_name);
	std::cout << channel_test->getChannelTopic() << std::endl;

	// change mode
	std::cout << std::endl;
	std::cout << "Change modes :" << std::endl;	
	std::cout << "Mode i is now : " << channel_test->isModeOn('i') << std::endl;
	std::cout << "Mode l is now : " << channel_test->isModeOn('l') << std::endl;
	std::cout << "Mode k is now : " << channel_test->isModeOn('k') << std::endl;
	std::cout << "Change mode to +ilk 100 coucou" << std::endl;	
	channel_test->setUnsetUserLimit(1,100);
	channel_test->setUnsetInviteMode(1);
	channel_test->setUnsetPassword(1, "coucou");
	std::cout << "Mode i is now : " << channel_test->isModeOn('i') << std::endl;
	std::cout << "Mode l is now : " << channel_test->isModeOn('l') << " with limit = "<< channel_test->getUserLimit() << std::endl;
	std::cout << "Mode k is now : " << channel_test->isModeOn('k') << " with pswd = "<< channel_test->getPassword() << std::endl;

	std::cout << std::endl;
	std::cout << "Change mode to -ilk" << std::endl;
	channel_test->setUnsetUserLimit(0,100);
	channel_test->setUnsetInviteMode(0);
	channel_test->setUnsetPassword(0, "coucou");	
	std::cout << "Mode i is now : " << channel_test->isModeOn('i') << std::endl;
	std::cout << "Mode l is now : " << channel_test->isModeOn('l') << std::endl;
	std::cout << "Mode k is now : " << channel_test->isModeOn('k') << std::endl;

	// add client
	std::cout << std::endl;
	std::cout << "Add '"<< client2.getUsername() <<"' then list clients :" << std::endl;	
	channel_test->addNewClient(client2);
	std::map<int,std::string> list_clients = channel_test->getClientsList();
	for (std::map<int,std::string>::iterator it=list_clients.begin(); it != list_clients.end();it++)
		std::cout<< "client fd, username : " << (it->first) << ":"<<it->second << std::endl;

	// make client2 operator
	std::cout << std::endl;
	std::cout << "Is '"<< client2.getUsername() <<"' an operator of #"<< channel_test->getChannelTopic()<< "? :" << channel_test->isOperator(client2.getSocket()) << std::endl;
	std::cout << "Make '"<< client2.getUsername() <<"' an operator of #"<< channel_test->getChannelTopic() << std::endl;
	channel_test->setUnsetOpPrivilege(1,client2.getUsername());
	std::cout << "Is '"<< client2.getUsername() <<"' an operator of #"<< channel_test->getChannelTopic()<< "? :" << channel_test->isOperator(client2.getSocket()) << std::endl;
	std::cout << "Unmake '"<< client2.getUsername() <<"' an operator of #"<< channel_test->getChannelTopic() << std::endl;
	channel_test->setUnsetOpPrivilege(0,client2.getUsername());
	std::cout << "Is '"<< client2.getUsername() <<"' an operator of #"<< channel_test->getChannelTopic()<< "? :" << channel_test->isOperator(client2.getSocket()) << std::endl;
	
	// del client
	std::cout << std::endl;
	std::cout << "Delete '"<< client2.getUsername() <<"' then list clients :" << std::endl;	
	channel_test->delClient(client2);
	list_clients = channel_test->getClientsList();
	for (std::map<int,std::string>::iterator it=list_clients.begin(); it != list_clients.end();it++)
		std::cout<< "client fd, username : " << (it->first) << ":"<<it->second << std::endl;

	std::cout << std::endl;
 	std::cout<< "Print list channels :"<<std::endl;	
	serv.print_list_channels();

	std::cout << std::endl;
 	std::cout<< "Delete last client then print list channels :"<<std::endl;	
	channel_test->delClient(client_creator);
	serv.print_list_channels();
}