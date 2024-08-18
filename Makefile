#-PATH FILES-#

SRC_DIR			= src
OBJ_DIR			= obj


#-SRC-#

SRC				= $(addprefix src/, $(SRCS))
OBJS			= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
D_OBJS			= mkdir -p $(@D)

SRCS		=	\
					main.cpp \
					Class/Server.cpp \
					Class/Client.cpp \
					Class/Channel.cpp \

#-UTILS-#

CXX 			= c++
CXXFLAGS 		= -Wall -Wextra -Werror
NAME 			= ft_irc
RM 				= rm -f
RMR				= rm -rf

#-RULES-#

all:			$(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
				$(D_OBJS)
				$(CXX) $(CXXFLAGS) -c -o $@ $<

$(NAME): 		$(OBJS)
				@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
				
clean:
				@$(RMR) $(OBJ_DIR)

fclean: 		clean
				@$(RM) $(NAME)

re:				fclean all

.PHONY : 		all clean fclean re
