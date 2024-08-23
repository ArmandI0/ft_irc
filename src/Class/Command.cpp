/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dboire <dboire@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:37:53 by dboire            #+#    #+#             */
/*   Updated: 2024/08/16 10:03:38 by dboire           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Command::Command(std::string& input, Client* client): _input(input), _client_requester(client)
{
}
Command::Command(std::string& input, Client* client, Server* server) : _input(input), _client_requester(client), _server(server)
{

}

Command::Command()
{
}

Command::Command(const Command& src)
{
	*this = src;
}

Command& Command::operator=(const Command& src)
{
	this->_server =src._server;
	this->_client_requester = src._client_requester;
	this->_input = src._input; 
	return(*this);
}

Command::~Command()
{

}
	
int	Command::parsingCommand()
{
	size_t 		pos = 0;
	std::string delimiter = "\r\n";

	while ((pos = this->_input.find(delimiter)) != std::string::npos)
	{
        std::string command = this->_input.substr(0, pos);
        this->_input.erase(0, pos + delimiter.length());
		this->execCommand(command);
    }
	return (0);
}

void	Command::execCommand(std::string cmd)
{
	std::istringstream 			iss(cmd);
	std::vector<std::string>	command;
	std::string					content;

	while (iss >> content)
		command.push_back(content);
	if (checkAndComp(command, 0, "PASS"))
		this->passCommand(command);
	else if (checkAndComp(command, 0, "NICK"))
		this->nickCommand(command);
	else if (checkAndComp(command, 0, "USER"))
		this->userCommand(command);
	else if (checkAndComp(command, 0, "JOIN"))
		this->execJoin(command);
	else if (checkAndComp(command, 0, "KICK"))
		this->execKick(command);
	if (this->_client_requester->getPass() && !this->_client_requester->getNick().empty() && !this->_client_requester->getUsername().empty())
	{
		this->_client_requester->setAuth();
		sendMessageToClient(this->_client_requester->getSocket(), "Welcome " + this->_client_requester->getNick() + "\r\n");
	}
}

Channel*	Command::createChannel(std::string& channel_name, Client* client_creator, Server* server)
{
		// Modifying operator
		Channel* new_channel = new Channel(channel_name, client_creator, server);
		server->setChannel(new_channel, channel_name);
		// Commande who
		std::string msg = "331 " + client_creator->getNick() + " " + channel_name + ":No topic is set\r\n";
		// std::string msg = "WHO " + channel_name + "\r\n";
		sendMessageToClient(this->_client_requester->getSocket(), msg);
		// sendMessageToClient(server.)
		std::cout << "Channel : " << channel_name << " created" << std::endl;
		return(new_channel);
}


/*			JOIN COMMAND		*/

void	Command::execJoin(std::vector<std::string> & command)
{

	
	Channel* channel = this->_server->getChannel(command[1]);
	if(channel)
	{
		if(channel->hasUser(_client_requester->getNick()) == true)
			sendMessageToClient(this->_client_requester->getSocket(), ERR_USERONCHANNEL(_client_requester->getNick(), command[1]));
		else
			channel->addClientToCh(this->_client_requester);
	}
	else
	{
		Channel* new_channel;
		new_channel = createChannel(command[1], _client_requester, this->_server);
	}
}

int	Command::parsingJoin(std::vector<std::string> & command)
{
	if(command[1][0] != '#' && command[1][0] != '&')
	{
		std::string msg = "403" + this->_client_requester->getSocket() + 32 + command[1] + ":No such channel\r\n";
		sendMessageToClient(this->_client_requester->getSocket(), msg);
	}
}


/*			KICK COMMAND		*/

void	Command::execKick(std::vector<std::string> & command)
{
	/* && !command[1].empty() && !command[2].empty() && command[3].empty()*/
	Channel* channel = this->_server->getChannel(command[1]);
	if(channel)
		channel->delClient(command[2]);
}



/* AUTHENTIFICATION */

int	Command::passCommand(std::vector<std::string> & password)
{
	std::cout << "PASS" << std::endl;
	if (this->_client_requester->getPass() == true)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_ALREADYREGISTRED);
	else if (password.size() == 1)
	{
		sendMessageToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS("PASS"));
	}
	else if(password[1].compare(this->_server->getPassword()) == 0 && password.size() == 2)
	{
		(this->_client_requester)->setPass();
		return 1;
	}
	return 0;
}

int	Command::nickCommand(std::vector<std::string> & nickname)
{
	std::cout << "NICK" << std::endl;
	if (nickname.size() == 1)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_NONICKNAMEGIVEN);
	else if (nickname.size() == 2 && nickname[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_") != std::string::npos)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_ERRONEUSNICKNAME(nickname[1]));	
	else if (nickname.size() == 2 && this->_server->findUserByNickname(nickname[1]) != NULL)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_NICKNAMEINUSE(nickname[1]));
	else if (nickname.size() == 2)
	{
		this->_client_requester->setNick(nickname[1]);
		this->_server->addNewNickname(nickname[1], &*(_client_requester));
	}
	return 0;
}

void Command::userCommand(std::vector<std::string> & username)
{
	std::cout << "USER" << std::endl;
	if (username.size() < 5)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS("USER"));
	else if (username.size() == 5 && this->_server->findUserByUsername(username[1]) != NULL)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_ALREADYREGISTRED);
	else if (username.size() == 5)
	{
		this->_client_requester->setUser(username[1]);
		this->_server->addNewUsername(username[1], &*(_client_requester));
	}
}


bool checkAndComp(std::vector<std::string> & entry, size_t i, const char* toComp)
{
	if (i == 0 && entry.empty())
		return false;
	else if (entry.size() <= i)
		return false;
	else if (entry[i].compare(std::string(toComp)) == 0)
		return true;
	else
		return false;
}

void sendMessageToClient(int fd, std::string error)
{
	int send = write(fd, error.c_str(), error.size());
	if (send < 0)
		std::cerr << "Message error" << std::endl; 
}

