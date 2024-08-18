SRCS		=	\
				UT_parsing.cpp \
				../../src/Class/Server.cpp \
				../../src/Class/Client.cpp \
				../../src/Class/Command.cpp \
				../../src/Class/Channel.cpp \

OBJS		= $(SRCS:%.cpp=%.o)

CXX 		= c++
CXXFLAGS 	= -Wall -Wextra -Werror -g
NAME 		= UT_channel
RM 			= rm -f
RMR			= rm -rf

#-RULES-#

all: $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
