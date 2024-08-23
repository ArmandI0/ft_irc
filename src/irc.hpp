
#ifndef irc_hpp
# define irc_hpp

#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define YELLOW  "\033[33m"
#define RESET "\033[0m"

# include <iostream>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <netdb.h>
# include <cstdlib>
# include <map>
# include <vector>
# include <list>
# include <unistd.h>
# include <sstream>
# include <csignal>


# include "Class/Server.hpp"
# include "Class/Client.hpp"
# include "Class/Channel.hpp"
# include "Class/Command.hpp"

/* AUTH ERROR */

#define ERR_NOTOPIC(nickname, channel_name) "331 " + nickname + " " + channel_name + ":No topic is set\r\n" RESET
#define RPL_NAMREPLY(nickname, channel_name) "353 " + nickname + " = " + channel_name + " : " RESET
#define ERR_ENDOFNAMES(nickname, channel_name) "366 " + nickname + " " + channel_name + " :End of /NAMES list\r\n" RESET
#define ERR_NOSUCHCHANNEL(nickname, channel_name) "403" + nickname + " " + channel_name + " :No such channel\r\n" RESET
#define ERR_NONICKNAMEGIVEN RED "431 :No nickname given\r\n" RESET
#define ERR_ERRONEUSNICKNAME(nickname) RED "432 " + nickname + " :Erroneus nickname\r\n" RESET
#define ERR_NICKNAMEINUSE(nickname) RED "433 " + nickname + " :Nickname is already in use\r\n" RESET
#define ERR_USERNOTINCHANNEL(nickname, t_nick, channel_name) "441 " + nickname + " " + channel_name  + " " + t_nick + " :They aren't on that channel\r\n" RESET
#define ERR_USERONCHANNEL(nickname, channel_name) "443 " + nickname + " :is already on channel: " + channel_name + "\r\n" RESET
#define ERR_NEEDMOREPARAMS(command) RED "461 " #command " :Not enough parameters\r\n" RESET
#define ERR_ALREADYREGISTRED RED "462 :You may not reregister\r\n" RESET
#define ERR_CHANOPRIVSNEEDED(nickname, channel_name) "482 " + nickname + " " + channel_name + " :You're not channel operator\r\n" RESET




#define MSG_WELCOME(nickname, channel_name) "Welcome " + nickname + " in " + channel_name + " !\r\n"

#define MSG_KICK(nickname, username, t_nickname, channel_name, reason) nickname + "!" + username + "@ft_irc KICK " + channel_name + " " + t_nickname + " :" + reason + "\r\n"

#endif