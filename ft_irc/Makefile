NAME = ircserv

SRCS =  src/IRC_Client.cpp \
		src/IRC_Server.cpp \
		src/IRC_Channel.cpp \
		src/handle_JOIN.cpp \
		src/handle_NICK.cpp \
		src/handle_PASS.cpp \
		src/handle_PRIVMSG.cpp \
		src/handle_USER.cpp \
		src/handle_INVITE.cpp \
		src/handle_KICK.cpp \
		src/IRC_EventHandler.cpp \
		src/IRC_Parser.cpp \
		src/main.cpp \
		src/Setup.cpp \

OBJS = $(SRCS:src/%.cpp=obj/%.o)

CC = c++
FLAGS = -Wall -Wextra -Werror -pedantic -std=c++98
INCLUDES = -I ./includes
RM = rm -rf

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

clean:
	@$(RM) obj

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
