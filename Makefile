# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/12 10:09:02 by nneronin          #+#    #+#              #
#    Updated: 2021/08/13 16:49:13 by nneronin         ###   ########.fr        #
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

NAME =	libft_malloc_$(HOSTTYPE).so

LINK =	libft_malloc.so

FLAGS =	-Wall -Werror -Wextra 

INCLUDES = -I ./include -I ./lib/libft -I ./lib/libpf

SRCS =	ft_malloc.c\
		ft_calloc.c\
		ft_realloc.c\
		ft_free.c\
		show_alloc_mem.c\
		zone_utils.c
		
SRC_DIR = ./srcs/
OBJ_DIR = ./objs/
OBJS := $(SRCS:.c=.o)
OBJS :=	$(addprefix $(OBJ_DIR),$(OBJS))

LIBS = ./lib/libft/libft.a ./lib/libpf/libpf.a

all: $(LIBS) $(NAME)
	@printf $(CYAN)"[INFO]	$(NAME) is up to date!\n"$(RESET)

$(NAME): $(OBJS)
	gcc $(OBJS) $(INCLUDES) $(LIBS) $(FLAGS) -fPIC -shared -o $(NAME)
	@/bin/rm -f $(LINK)
	@ln -s $(NAME) $(LINK)
	@echo "$(NAME) was successfully created."

$(OBJS): $(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@/bin/mkdir -p $(OBJ_DIR)
	$(CC) -c $(FLAGS) $(INCLUDES) $< -o $@

$(LIBS):
	@make -C ./lib/libft
	@make -C ./lib/libpf

clean:
	-/bin/rm -f $(OBJS)
	/usr/bin/find . -name "obj" -maxdepth 1 -type d -empty -delete
	@printf $(CYAN)"[INFO]	$(NAME) cleaned\n"$(RESET)

fclean: clean
	/bin/rm -f $(NAME) $(LINK)
	/bin/rm -f ./*.so

re: fclean all

.PHONY: clean, all, fclean, re
