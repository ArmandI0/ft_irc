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
	this->_client_requester = src._client_requester;
	this->_input = src._input; 
	return(*this);
}

Command::~Command()
{
}


void error_message(std::string error)
{
	std::cout << "Syntax error : " << error << std::endl;
}
	
void	Command::createChannel(std::string& channel_name, Client& client_creator, Server* server)
{
		// Modifying operator
		Channel new_channel(channel_name, client_creator, server);
		server->setChannel(new_channel, channel_name);
		// Commande who
		std::string msg = "331 " + client_creator.getNick() + " " + channel_name + ":No topic is set\r\n";
		// std::string msg = "WHO " + channel_name + "\r\n";
		sendMessageToClient(this->_client_requester->getSocket(), msg);
		// sendMessageToClient(server.)
		std::cout << "Channel : " << channel_name << " created" << std::endl;
}

void	Command::execJoin()
{
	Channel* channel = this->_server->hasChannel(this->_command[1]);
	if(channel)
	{
		if(channel->hasUser(_client_requester->getNick()) == true)
			sendMessageToClient(this->_client_requester->getSocket(), ERR_USERONCHANNEL(_client_requester->getNick(), this->_command[1]));
		else
			channel->addNewClient(this->_client_requester);
	}
	else
	{
		createChannel(this->_command[1], *this->_client_requester, this->_server);
		channel->addNewClient(this->_client_requester);
	}
}

void	Command::execNick()
{
	std::string authorized_first_character = "-^~";
	std::string invalid_chars = ",:!@.";
	if(!isalpha(this->_command[1][0]) || (authorized_first_character.find(this->_command[1][0]) == std::string::npos))
	{
		std::cout << "First letter of the Nick must be a letter or - ^ ~" << std::endl;
		return ;
	}
	for(char c : this->_command[1])
	{
		if(invalid_chars.find(c) != std::string::npos)
		{
			std::cout << "You have placed a forbidden character '" << c << "' in your Nickname" << std::endl;
			return ;
		}
	}
	//4. Nickname Change Notification TO DO
		//Description: When a user changes their nickname, the IRC server broadcasts this change to all other users in the channels where the user is present.
	this->_client_requester->setNick(this->_command[1]);
}

void	Command::execKick()
{
	Channel* channel = this->_server->hasChannel(this->_command[1]);
	if(channel)
		channel->delClient(this->_command[2]);
}

void Command::server_msg()
{
	std::istringstream iss(this->_input);
	std::string content;

	while (iss >> content)
		this->_command.push_back(content);

	if (this->_command[0] == "JOIN" && !this->_command[1].empty())
		execJoin();
	if (this->_command[0] == "NICK" && !this->_command[1].empty() && this->_command[2].empty()) // OK
		execNick();
	if (checkAndComp(_command, 0, "KICK") && !this->_command[1].empty() && !this->_command[2].empty() && this->_command[3].empty())
		execKick();
}



/* AUTHENTIFICATION */

int	Command::serverAuth()
{
	size_t pos = 0;
	std::string delimiter = "\r\n";

	while ((pos = this->_input.find(delimiter)) != std::string::npos)
	{
        std::string command = this->_input.substr(0, pos);
        this->_input.erase(0, pos + delimiter.length());
		this->doCommandAuth(command);
    }
	return (0);
}
void	Command::doCommandAuth(std::string cmd)
{
	std::istringstream 			iss(cmd);
	std::string					content;

	while (iss >> content)
		_command.push_back(content);
	if (checkAndComp(_command, 0, "PASS"))
		this->passCommand(_command);
	else if (checkAndComp(_command, 0, "NICK"))
		this->nickCommand(_command);
	else if (checkAndComp(_command, 0, "USER"))
		this->userCommand(_command);
	if (this->_client_requester->getPass() && !this->_client_requester->getNick().empty() && !this->_client_requester->getUsername().empty())
	{
		this->_client_requester->setAuth();
		sendMessageToClient(this->_client_requester->getSocket(), "Welcome " + this->_client_requester->getNick() + "\r\n");
	}
}

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
		std::cout << "Nickname : " << this->_client_requester->getNick() << std::endl;
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

