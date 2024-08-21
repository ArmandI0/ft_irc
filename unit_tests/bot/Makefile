SRCS		=	\
				UT_bot.cpp \
				../../bonus/Bot.cpp \

OBJS		= $(SRCS:%.cpp=%.o)

CXX 		= c++
CXXFLAGS 	= -Wall -Wextra -Werror -g 
NAME 		= UT_bot
RM 			= rm -f
RMR			= rm -rf

API_FILE = api_irc.txt
API_KEY = $(shell cat $(API_FILE))

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
