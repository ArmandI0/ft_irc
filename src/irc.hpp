
#ifndef irc_hpp
# define irc_hpp

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

#define ERR_NOTOPIC(nickname, channel_name) "331 " + nickname + " " + channel_name + ":No topic is set\r\n" 
#define RPL_TOPIC(nickname, channel_name, topic) "332 " + nickname + " " + channel_name + " :" + topic + "\r\n" 
#define RPL_NAMREPLY(nickname, channel_name) "353 " + nickname + " = " + channel_name + " : " 						 // Pas de \r\n
#define ERR_ENDOFNAMES(nickname, channel_name) "366 " + nickname + " " + channel_name + " :End of /NAMES list\r\n" 
#define	ERR_NOSUCHNICK(nickname, t_nickname) "401 " + nickname + " " + t_nickname + " :No such nick\r\n" 
#define ERR_NOSUCHCHANNEL(nickname, channel_name) "403 " + nickname + " " + channel_name + " :No such channel\r\n" 
#define ERR_NONICKNAMEGIVEN  "431 :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nickname)  "432 " + nickname + " :Erroneus nickname\r\n" 
#define ERR_NICKNAMEINUSE(nickname)  "433 " + nickname + " :Nickname is already in use\r\n" 
#define ERR_USERNOTINCHANNEL(nickname, t_nick, channel_name) "441 " + nickname + " " + channel_name  + " " + t_nick + " :They aren't on that channel\r\n" 
#define ERR_USERONCHANNEL(nickname, channel_name) "443 " + nickname + " :is already on channel: " + channel_name + "\r\n" 
#define ERR_NEEDMOREPARAMS(nickname, command)  "461 " + nickname + " " + command + " :Not enough parameters\r\n" 
#define ERR_ALREADYREGIST  "462 :You may not reregister\r\n" 
#define ERR_CHANNELISFULL(nickname, channel_name) "471 " + nickname + " " + channel_name + " :Cannot join channel (channel is full)\r\n" 
#define ERR_INVITEONLYCHAN(nickname, channel_name) "473 " + nickname + " " + channel_name + " :Cannot join channel (+i)\r\n" 
#define ERR_CHANOPRIVSNEEDED(nickname, channel_name) "482 " + nickname + " " + channel_name + " :You're not channel operator\r\n" 
#define ERR_UMODEUNKNOWNFLAG(nickname, mode) "501 " + nickname + " :Unknown " + mode + " flag\r\n" 

#define ERR_NORECIPIENT(command)  "411 :No recipient given " command " \r\n" 
#define ERR_NOTEXTTOSEND  "412 :No text to send\r\n" 
#define ERR_CANNOTSENDTOCHAN(channel_name)  "404 " + channel_name + ":Cannot send to channel\r\n" 
#define ERR_TOOMANYTARGETS  "407 :Too many recipients\r\n" 


#define MSG_MODECHANGE(channel_name, mode) "MODE " + channel_name + " " + mode + "\r\n" 
#define MSG_NEWEOPERONCHANNEL(nickname, target, channel_name) nickname + ": " + target + " Has been promoted to operator on " + channel_name + " Congratulations !\r\n" 
#define MSG_KEYONCHANNEL(channel_name, add_remove) "MODE " + channel_name + " " + add_remove "k\r\n"  
#define MSG_REMOVEOP(nickname, t_nickname, channel_name) "User :" + t_nickname + " has been removed Operator rights in " + channel_name + " from " + nickname + "\r\n" 
#define MSG_WELCOME(nickname, channel_name) "Welcome " + nickname + " in " + channel_name + " !\r\n" 
#define	MSG_INVITER(nickname, t_nickname, channel_name) "341 " + nickname + " " + t_nickname + " " + channel_name + "\r\n" 
#define	MSG_INVITEE(nickname, t_nickname, channel_name) ":" + nickname + " INVITE " + t_nickname + " :" + channel_name + "\r\n" 

#define MSG_KICK(nickname, username, t_nickname, channel_name, reason) nickname + "!" + username + "@ft_irc KICK " + channel_name + " " + t_nickname + " :" + reason + "\r\n" 

#define ERR_NOTREGISTER  "TU ES PAS CONNECTE CONNARD ! METS LE PUTAIN DE MOT DE PASSE\r\n"  

#endif