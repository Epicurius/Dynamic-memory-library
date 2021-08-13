# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/12 10:09:02 by nneronin          #+#    #+#              #
#    Updated: 2021/08/13 14:54:16 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED				:= "\e[0;31m"
GREEN			:= "\e[0;32m"
YELLOW			:= "\e[0;33m"
BLUE			:= "\e[0;34m"
MAGENTA			:= "\e[0;35m"
CYAN			:= "\e[0;36m"
RESET			:= "\e[0m"

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libmem.a

INCLUDES = -I . -I ./lib/libft -I ../libs/libpf

SRCS =	./srcs/main.c\
		./srcs/ft_malloc.c\
		./srcs/ft_calloc.c\
		./srcs/ft_realloc.c\
		./srcs/ft_free.c\
		./srcs/show_alloc_mem.c\
		./srcs/zone_utils.c	

OBJS = $(SRCS:.c=.o)
FLAGS = -Wall -Wextra -Werror

LIBS = ./lib/libft/libft.a ../libs/libpf/libpf.a

all: $(LIBS) $(NAME)
	@printf $(CYAN)"[INFO]	$(NAME) is up to date!\n"$(RESET)

$(LIBS):
	@make -C ./lib/libft
	@make -C ../libs/libpf

$(NAME): $(SRCS)
	@gcc -o $(NAME) $(SRCS) $(LIBS) $(INCLUDES) -pthread
	@echo "$(NAME) was successfully created."

clean:
	@rm -f $(OBJS)
	@printf $(CYAN)"[INFO]	$(NAME) cleaned\n"$(RESET)

fclean: clean
	@rm -f $(NAME)
	@make re -C ../libs/libpf

re: fclean all

.PHONY: clean, all, fclean, re
