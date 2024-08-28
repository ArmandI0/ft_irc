#-PATH FILES-#

SRC_DIR         = src
OBJ_DIR         = obj

#-SRC-#

SRCS            =   main.cpp \
                    Class/Server.cpp \
                    Class/Server_ChannelManagement.cpp \
                    Class/Server_ClientManagement.cpp \
                    Class/Client.cpp \
                    Class/Command.cpp \
                    Class/Command_Auth.cpp \
                    Class/Channel.cpp

SRCS_BOT        =   Bonus/Bot.cpp \
					Bonus/main.cpp \

OBJS            = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
OBJS_BOT        = $(SRCS_BOT:%.cpp=$(OBJ_DIR)/%.o)

#-UTILS-#

CXX             = c++
CXXFLAGS        = -Wall -Wextra -Werror -g -std=c++98
NAME            = ft_irc
BOT             = ft_irc_bot
RM              = rm -f
RMR             = rm -rf

#-RULES-#

all:            $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
				@mkdir -p $(dir $@)
				$(CXX) $(CXXFLAGS) -c -o $@ $<

$(NAME):        $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
				$(CXX) $(CXXFLAGS) $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o)) -o $(NAME)

$(BOT):         $(addprefix $(OBJ_DIR)/, $(SRCS_BOT:.cpp=.o))
				$(CXX) $(CXXFLAGS) $(addprefix $(OBJ_DIR)/, $(SRCS_BOT:.cpp=.o)) -o $(BOT)

bot:			$(BOT)

clean:
				$(RMR) $(OBJ_DIR)

fclean:			clean
				$(RM) $(NAME) $(BOT)

re:				fclean all

.PHONY:			all clean fclean re bot