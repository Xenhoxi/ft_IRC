# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smunio <smunio@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/28 15:35:39 by ljerinec          #+#    #+#              #
#    Updated: 2024/04/29 18:37:29 by smunio           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++
CFLAGS = -Wall -Werror -Wextra -g3 -std=c++98

SOURCES =	srcs/main.cpp \
			srcs/user/user.class.cpp \
			srcs/user/userGeters.class.cpp \
			srcs/user/userCommands.class.cpp \
			srcs/server/server.class.cpp \
			srcs/server/serverGeters.class.cpp \
			srcs/server/serverCommands.class.cpp \
			srcs/channel/channel.class.cpp \
			srcs/channel/channelGeters.class.cpp \
			srcs/channel/channelCommands.class.cpp \
			srcs/channel/channelMode.class.cpp

OBJ_DIR = objs/
OBJECTS = $(SOURCES:.cpp=.o)

FT_IRC = ircserv

INCLUDES_DIR = inc/

####################COMPILATION STYLING####################

TOTAL_FILES = $(words $(SOURCES))
CURRENT_FILE = 0
BAR_WIDTH = 30

PRINT_NAME		:= ft_irc
PRINT_PREFIX	:=	\033[1m\033[38;5;240m[\033[0m\033[38;5;250m$(PRINT_NAME)\033[1m\033[38;5;240m] \033[38;5;105m~\033[0m

all: $(FT_IRC)

%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDES_DIR)
	@$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))
	@$(eval PROGRESS=$(shell echo $$(($(CURRENT_FILE) * $(BAR_WIDTH) / $(TOTAL_FILES)))))
	@$(eval REMAINING=$(shell echo $$(($(BAR_WIDTH) - $(PROGRESS)))))

	@printf "$(PRINT_PREFIX) \033[1;33m[$(CURRENT_FILE)/$(TOTAL_FILES)] ["
	@printf "%${PROGRESS}s" | tr ' ' '■'
	@printf "%${REMAINING}s" | tr ' ' ' '
	@printf "]\r\033[0m"


$(FT_IRC): $(OBJECTS)
	@$(CC) $(CFLAGS) -o $(FT_IRC) $(OBJECTS)
	@printf "$(PRINT_PREFIX) \033[1;32m[$(CURRENT_FILE)/$(TOTAL_FILES)] ["
	@printf "%${PROGRESS}s" | tr ' ' '■'
	@printf "%${REMAINING}s" | tr ' ' ' '
	@printf "][OK]\n\033[0m"

clean:
	@rm -rf $(OBJECTS)

fclean: clean
	@rm -f $(FT_IRC)

re: fclean all

.PHONY: all clean fclean re
