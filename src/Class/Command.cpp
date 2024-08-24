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
	else if (checkAndComp(command, 0, "MODE"))
		this->execMode(command);
	if (this->_client_requester->getPass() && !this->_client_requester->getNick().empty() && !this->_client_requester->getUsername().empty())
	{
		this->_client_requester->setAuth();
		// sendMessageToClient(this->_client_requester->getSocket(), "Welcome " + this->_client_requester->getNick() + "\r\n");
	}
}

bool		Channel::checkIfOp(std::string name)
{
	for(std::map<std::string, Client *>::iterator it = _operator.begin(); it != _operator.end(); ++it)
	{
		if(it->first == name)
			return (true);
	}
	return (false);
}

Channel*	Command::createChannel(std::string & channel_name, Client* client_creator, Server* server)
{
		Channel* new_channel = new Channel(channel_name, client_creator, server);
		
		server->setChannel(new_channel, channel_name);
		return(new_channel);
}

/*			MODE COMMAND		*/

void	Command::execOpMode(Channel * channel, Client * client, int remove)
{
	if(remove == false)
	{
		channel->addClientToOp(client);
		channel->sendMessageToAllClient(MSG_NEWEOPERONCHANNEL(client->getNick(), channel->getName()));
	}
	else
	{
		channel->delClientToOp(client);
		channel->sendMessageToAllClient(MSG_REMOVEOP(client->getNick(), client->getNick(),  channel->getName()));
	}
}

void	Command::execKeyMode(Channel * channel, std::string key, int remove)
{
	if(remove == false)
	{
		channel->setKey(key);
		channel->sendMessageToAllClient(MSG_KEYONCHANNEL(channel->getName(), "+k"));
	}
	else
	{
		channel->setKey("");
		channel->sendMessageToAllClient(MSG_KEYONCHANNEL(channel->getName(), "-k"));
	}
}

void	Command::execLimitMode(Channel * channel, std::string limit, int remove)
{
	if(remove == false)
	{
		channel->setLimit(limit);
		channel->sendMessageToAllClient(MSG_MODECHANGE(channel->getName(), "+l"));
	}
	else
	{
		channel->setLimit("-1");
		channel->sendMessageToAllClient(MSG_MODECHANGE(channel->getName(), "-l"));
	}
}

void	Command::execTopicMode(Channel * channel, int remove)
{
	channel->setTopic(remove);
	if(remove == true)
		channel->sendMessageToAllClient(MSG_MODECHANGE(channel->getName(), "-t"));
	else
		channel->sendMessageToAllClient(MSG_MODECHANGE(channel->getName(), "+t"));
}

void	Command::execInviteMode(Channel * channel, int remove)
{
	channel->setInvite(remove);
	if(remove == true)
		channel->sendMessageToAllClient(MSG_MODECHANGE(channel->getName(), "-i"));
	else
		channel->sendMessageToAllClient(MSG_MODECHANGE(channel->getName(), "+i"));

}

void	Command::execMode(std::vector<std::string> & command)
{
	if(command.size() < 2)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS(this->_client_requester->getNick(), command[0]));
	else
	{
		if(command[1][0] == '#' || command[1][0] == '&')
		{
			Channel* channel = this->_server->getChannel(command[1]);
			if(channel)
			{
				if(command.size() == 4)
				{
					Client * client = this->_server->findUserByNickname(this->_client_requester->getNick());
					if(client)
					{
						if(channel->checkIfOp(client->getNick()) == true)
						{
							if(command[2][0] == '+' || command[2][0] == '-')
							{
								int i = 0;
								if(command[2][0] == '-')
									i++;
								if(command[2][1] == 'o')
									execOpMode(channel, client, i);
								else if(command[2][1] == 'k')
								{
									if(i != 1)
										execKeyMode(channel, command[3], 0);
									else
										sendMessageToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS(this->_client_requester->getNick(), command[2]));
								}
								else if(command[2][1] == 'l')
								{
									if(i != 1)
										execLimitMode(channel, command[3], 0);
								}
								else if(command[2][1] == 't')
									sendMessageToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS(this->_client_requester->getNick(), command[2]));
								else if(command[2][1] == 'i')
									sendMessageToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS(this->_client_requester->getNick(), command[2]));
								else
									sendMessageToClient(this->_client_requester->getSocket(), ERR_UMODEUNKNOWNFLAG(this->_client_requester->getNick(), command[3]));
							}
							else
								sendMessageToClient(this->_client_requester->getSocket(), ERR_UMODEUNKNOWNFLAG(this->_client_requester->getNick(), command[3]));
						}
						else
							sendMessageToClient(this->_client_requester->getSocket(), ERR_CHANOPRIVSNEEDED(this->_client_requester->getNick(), command[1]));
					}
				}
				else
				{
					if(command[2][0] == '-' || command[2][0] == '+')
					{
						int i = 0;
						if(command[2][0] == '-')
							i++;
						if(command[2][1] == 'o')
							sendMessageToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS(this->_client_requester->getNick(), command[2]));
						else if(command[2][1] == 'k')
								execKeyMode(channel, "", i);
						else if(command[2][1] == 'l')
							execLimitMode(channel, "", i);
						else if(command[2][1] == 't')
							execTopicMode(channel, i);
						else if(command[2][1] == 'i')
							execInviteMode(channel, i);
						else
							sendMessageToClient(this->_client_requester->getSocket(), ERR_UMODEUNKNOWNFLAG(this->_client_requester->getNick(), command[2]));
					}
				}
			}
			else
				sendMessageToClient(this->_client_requester->getSocket(), ERR_NOSUCHCHANNEL(this->_client_requester->getNick(), command[1]));
		}
		else
		{
			Client * client = this->_server->findUserByNickname(command[1]);
			if(client)
			{
				
			}
			else
				sendMessageToClient(this->_client_requester->getSocket(), ERR_NOSUCHNICK(this->_client_requester->getNick(), command[1]));
		}
	}
}



/*			JOIN COMMAND		*/

void	Command::execJoin(std::vector<std::string> & command)
{
	std::cout << command[1] << std::endl;
	if(command.size() < 2)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS(this->_client_requester->getNick(), command[0]));
	else if(!command[1].empty())
	{
		if(command[1][0] != '#' && command[1][0] != '&')
			sendMessageToClient(this->_client_requester->getSocket(), ERR_NOSUCHCHANNEL(this->_client_requester->getNick(), command[1]));
		else
		{
			Channel* channel = this->_server->getChannel(command[1]);
			if(channel)
			{
				if(channel->hasUser(_client_requester->getNick()) == true)
					sendMessageToClient(this->_client_requester->getSocket(), ERR_USERONCHANNEL(_client_requester->getNick(), command[1]));
				else
				{
					if(channel->checkLimitUser() == true)
						;
					if(channel->checkInvite(_client_requester->getNick()) == true)
						channel->addClientToCh(this->_client_requester);
					else
						sendMessageToClient(this->_client_requester->getSocket(), ERR_CHANNELISFULL(_client_requester->getNick(), channel->getName()));
				}
			}
			else
			{
				std::cout << command[1] << std::endl;
				createChannel(command[1], _client_requester, _server);
			}
		}
	
	}
}

/*			KICK COMMAND		*/

void	Command::execKick(std::vector<std::string> & command)
{
	if(command[1].empty() || command[2].empty())
		sendMessageToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS(this->_client_requester->getNick(), command[0]));
	else
	{
		Channel* channel = this->_server->getChannel(command[1]);
		if(channel)
		{
			if(channel->checkIfOp(this->_client_requester->getNick()) == false)
				sendMessageToClient(this->_client_requester->getSocket(), ERR_USERONCHANNEL(_client_requester->getNick(), command[1]));
			else
			{
				if(channel->hasUser(command[2]) == false)
					sendMessageToClient(this->_client_requester->getSocket(), ERR_USERNOTINCHANNEL(_client_requester->getNick(), command[2], command[1]));
				else
					channel->kickClient(this->_client_requester, command[2], command[3]);
			}
		}
		else
			sendMessageToClient(this->_client_requester->getSocket(), ERR_NOSUCHCHANNEL(_client_requester->getNick(), channel->getName()));
	}
}



/* AUTHENTIFICATION */

int	Command::passCommand(std::vector<std::string> & password)
{
	std::cout << "PASS" << std::endl;
	if (this->_client_requester->getPass() == true)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_ALREADYREGISTRED);
	else if (password.size() == 1)
	{
		sendMessageToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS(this->_client_requester->getNick() ,"PASS"));
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
		this->_server->addNewNickname(nickname[1], _client_requester);
	}
	return 0;
}

void Command::userCommand(std::vector<std::string> & username)
{
	std::cout << "USER" << std::endl;
	if (username.size() < 5)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_NEEDMOREPARAMS(this->_client_requester->getNick(), "USER"));
	else if (username.size() == 5 && this->_server->findUserByUsername(username[1]) != NULL)
		sendMessageToClient(this->_client_requester->getSocket(), ERR_ALREADYREGISTRED);
	else if (username.size() == 5)
	{
		this->_client_requester->setUser(username[1]);
		this->_server->addNewUsername(username[1], _client_requester);
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

