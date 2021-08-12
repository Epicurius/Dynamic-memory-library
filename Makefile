# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/12 10:09:02 by nneronin          #+#    #+#              #
#    Updated: 2021/08/12 16:27:50 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED				:= "\e[0;31m"
GREEN			:= "\e[0;32m"
YELLOW			:= "\e[0;33m"
BLUE			:= "\e[0;34m"
MAGENTA			:= "\e[0;35m"
CYAN			:= "\e[0;36m"
RESET			:= "\e[0m"

NAME = libmem.a

INCLUDES = -I ./libmem.h -I ./lib/libft -I ./lib/libpf

SRCS =	main.c\
		ft_malloc.c\
		ft_free.c\
		show_alloc_mem.c\
		zone_utils.c	

OBJS = $(SRCS:.c=.o)
FLAGS = -Wall -Wextra -Werror

LIBS = ./lib/libft/libft.a ./lib/libpf/libpf.a

all: $(LIBS) $(NAME)
	@printf $(CYAN)"[INFO]	$(NAME) is up to date!\n"$(RESET)

$(LIBS):
	@make -C ./lib/libft
	@make -C ./lib/libpf

$(NAME): $(SRCS)
	@gcc -o $(NAME) $(SRCS) $(LIBS) $(INCLUDES) -pthread
	@echo "$(NAME) was successfully created."

clean:
	@rm -f $(OBJS)
	@printf $(CYAN)"[INFO]	$(NAME) cleaned\n"$(RESET)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean, all, fclean, re
