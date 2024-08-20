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
	

void	Command::execJoin()
{
	if(this->_server->hasChannel(this->_command[1]) == true)
	{
		this->_server->addUserToChannel(this->_command[1], _client_requester);
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

void Command::server_msg()
{

	std::istringstream iss(this->_input);
	std::string content;

	while (std::getline(iss, content))
		this->_command.push_back(content);

	if (this->_command[0] == "/QUIT" && this->_command[1].empty())
		; // function to exit the irc server and free the socket

	if (this->_command[0] == "/JOIN" && !this->_command[1].empty() && this->_command[2].empty())
		execJoin();

	if (this->_command[0] == "NICK" && !this->_command[1].empty() && this->_command[2].empty()) // OK
		execNick();
	// // if(input[0] == ':')
	// // 	nickname_change(channels);
	// if(operators == 1)
	// {
	// 	// if (input.compare(0, 6, "/KICK ") == 0)
	// 	// {
	// 	// 	kick_command(channels);
	// 	// }
	// 	if(command == "/INVITE")
	// 	{
	// 		std::cout << "/INVITE" << std::endl;
	// 	}
	// 	if(command == "/TOPIC")
	// 	{
	// 		std::cout << "/TOPIC" << std::endl;
	// 	}
	// 	if (command == "MODE ") // Need to be exactly like : MODE #channel +mode target_user / Example :Carol!carol@irc.example.com MODE #chatroom +v Dave
	// 	{
	// 		mode_command(channels);
	// 		//go in function to see sub div mode
	// 	}
	// 	if(input == "EXIT")
	// 		exit(0);
	// }

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
	if (this->_client_requester->getPass() && !this->_client_requester->getNick().empty() && !this->_client_requester->getUsername().empty())
		this->_client_requester->setAuth();
}

int	Command::passCommand(std::vector<std::string> & password)
{
	if (this->_client_requester->getPass() == true)
		sendErrorToClient(this->_client_requester->getSocket(), ERR_ALREADYREGISTRED);
	else if (password.size() == 1)
	{
		sendErrorToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS("PASS"));
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
	if (nickname.size() == 1)
		sendErrorToClient(this->_client_requester->getSocket(), ERR_NONICKNAMEGIVEN);
	else if (nickname.size() == 2 && nickname[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_") != std::string::npos)
		sendErrorToClient(this->_client_requester->getSocket(), ERR_ERRONEUSNICKNAME(nickname[1]));	
	else if (nickname.size() == 2 && this->_server->findUserByNickname(nickname[1]) != NULL)
		sendErrorToClient(this->_client_requester->getSocket(), ERR_NICKNAMEINUSE(nickname[1]));
	else if (nickname.size() == 2)
	{
		this->_client_requester->setNick(nickname[1]);
		this->_server->addNewNickname(nickname[1], &*(_client_requester));
	}
	return 0;
}

void Command::userCommand(std::vector<std::string> & username)
{
	if (username.size() < 5)
		sendErrorToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS("USER"));
	else if (username.size() == 5 && this->_server->findUserByUsername(username[1]) != NULL)
		sendErrorToClient(this->_client_requester->getSocket(), ERR_ALREADYREGISTRED);
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

void sendErrorToClient(int fd, std::string error)
{
	int send = write(fd, error.c_str(), error.size());
	if (send < 0)
		std::cerr << "Message error" << std::endl; 
}

