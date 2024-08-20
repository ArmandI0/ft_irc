
#ifndef irc_hpp
# define irc_hpp

#define RED "\033[31m"
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

# include "Class/Server.hpp"
# include "Class/Client.hpp"
# include "Class/Channel.hpp"
# include "Class/Command.hpp"

/* AUTH ERROR */

#define ERR_NONICKNAMEGIVEN RED "431 :No nickname given\r\n" RESET
#define ERR_ERRONEUSNICKNAME(nickname) RED "432 " + nickname + " :Erroneus nickname\r\n" RESET
#define ERR_NICKNAMEINUSE(nickname) RED "433 " + nickname + " :Nickname is already in use\r\n" RESET
#define ERR_NEEDMOREPARAMS(command) RED "461 " #command " :Not enough parameters\r\n" RESET
#define ERR_ALREADYREGISTRED RED "462 :You may not reregister\r\n" RESET


#endif