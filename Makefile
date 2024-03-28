# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ljerinec <ljerinec@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/28 15:35:39 by ljerinec          #+#    #+#              #
#    Updated: 2024/03/28 15:39:41 by ljerinec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = g++
CFLAGS = -Wall -Werror -Wextra

SOURCES =	sources/main.c \

OBJ_DIR = object/
OBJECTS = $(SOURCES:.cpp=.o)

FT_IRC = ft_irc

INCLUDES_DIR = includes/

####################COMPILATION STYLING####################

TOTAL_FILES = $(words $(SOURCES))
CURRENT_FILE = 0
BAR_WIDTH = 30

PRINT_NAME		:= ft_irc
PRINT_PREFIX	:=	\033[1m\033[38;5;240m[\033[0m\033[38;5;250m$(PRINT_NAME)\033[1m\033[38;5;240m] \033[38;5;105m~\033[0m

all: $(FT_IRC)

%.o: %.cpp
	@$(CC) $(CFLAGS) -cpp $< -o $@ -I $(INCLUDES_DIR)
	@$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))
	@$(eval PROGRESS=$(shell echo $$(($(CURRENT_FILE) * $(BAR_WIDTH) / $(TOTAL_FILES)))))
	@$(eval REMAINING=$(shell echo $$(($(BAR_WIDTH) - $(PROGRESS)))))

	@printf "$(PRINT_PREFIX) \033[1;33m[$(CURRENT_FILE)/$(TOTAL_FILES)] ["
	@printf "%${PROGRESS}s" | tr ' ' '■'
	@printf "%${REMAINING}s" | tr ' ' ' '
	@printf "]\r\033[0m"

$(CUB3D): $(OBJECTS)
	@$(CC) $(CFLAGS) -o $(FT_IRC) $(OBJECTS)
	@printf "$(PRINT_PREFIX) \033[1;32m[$(CURRENT_FILE)/$(TOTAL_FILES)] ["
	@printf "%${PROGRESS}s" | tr ' ' '■'
	@printf "%${REMAINING}s" | tr ' ' ' '
	@printf "][OK]\n\033[0m"

clean:
	@make -C includes/libft clean
	@rm -f $(OBJECTS)
	@rm -f *.o
	@rm -f */*.o
	@rm -f */*/*.o

fclean: clean
	@make -C includes/libft fclean
	@rm -f $(FT_IRC)
	@rm -f "minishell *"

re: fclean all

.PHONY: all clean fclean re