NAME = ircserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic

SRC = main.cpp IRC_Server.cpp Setup.cpp EventHandler.cpp handle_PASS.cpp handle_JOIN.cpp handle_NICK.cpp handle_USER.cpp handle_PRIVMSG.cpp

OBJ = $(SRC:.cpp=.o)

ifeq ($(OS),Windows_NT)
	RM = del /f
	NAME := $(NAME).exe
else
	RM = rm -f
endif

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
