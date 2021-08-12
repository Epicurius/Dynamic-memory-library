# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/12 10:09:02 by nneronin          #+#    #+#              #
#    Updated: 2021/08/12 14:44:46 by nneronin         ###   ########.fr        #
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

INCLUDES = -I ./libmem.h -I ./libs/libft -I ./libs/libpf

SRCS =	main.c\
		ft_malloc.c\
		ft_free.c\
		show_alloc_mem.c\
		zone_utils.c	

OBJS = $(SRCS:.c=.o)
FLAGS = -Wall -Wextra -Werror

all: $(NAME)
	@printf $(CYAN)"[INFO]	$(NAME) is up to date!\n"$(RESET)

$(NAME): $(SRCS)
	@gcc -o $(NAME) $(SRCS) $(INCLUDES) -pthread ./libs/libft/libft.a ./libs/libpf/libpf.a
	@echo "$(NAME) was successfully created."

clean:
	@rm -f $(OBJS)
	@printf $(CYAN)"[INFO]	$(NAME) cleaned\n"$(RESET)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean, all, fclean, re
