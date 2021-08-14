# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/12 10:09:02 by nneronin          #+#    #+#              #
#    Updated: 2021/08/14 14:53:48 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED				:= "\e[0;31m"
GREEN			:= "\e[0;32m"
YELLOW			:= "\e[0;33m"
BLUE			:= "\e[0;34m"
MAGENTA			:= "\e[0;35m"
CYAN			:= "\e[0;36m"
RESET			:= "\e[0m"

SHELL_NAME	:= $(shell uname -s)

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

PWD			=	$(shell pwd)

NAME 		=	libft_malloc_$(HOSTTYPE).so

LINK 		=	libft_malloc.so

FLAGS 		=	#-Wall -Werror -Wextra 

SDL_DIR		=	./SDL_Frameworks
FRAMEWORK 	=	-F $(PWD)/frameworks -framework SDL2 -framework SDL2_ttf -Wl,-rpath $(PWD)/frameworks

INCLUDES 	=	-I ./include \
				-I ./lib/libft \
				-I ./lib/libpf \
				-I ./SDL2
				
LIBS		=	./lib/libft/libft.a \
				./lib/libpf/libpf.a 

SRCS 		=	ft_malloc.c\
				ft_calloc.c\
				ft_realloc.c\
				ft_free.c\
				show_alloc_mem.c\
				zone_utils.c\
				visualizer.c\
				draw.c\
				size_to_str.c\
				update_memory_visualizer.c\
				draw_memory.c\
		
SRC_DIR = ./srcs/
OBJ_DIR = ./objs/
OBJS := $(SRCS:.c=.o)
OBJS :=	$(addprefix $(OBJ_DIR),$(OBJS))


all: framework $(LIBS) $(NAME)
	@printf $(CYAN)"[INFO]	$(NAME) is up to date!\n"$(RESET)

$(NAME): $(OBJS)
	@gcc $(FRAMEWORK) $(OBJS) $(INCLUDES) $(LIBS) $(FLAGS) -fPIC -shared -o $(NAME)
	@/bin/rm -f $(LINK)
	@ln -s $(NAME) $(LINK)
	@echo "$(NAME) was successfully created."

$(OBJS): $(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@/bin/mkdir -p $(OBJ_DIR)
	@$(CC) -c $(FLAGS) -F ./frameworks $(INCLUDES) $< -o $@

$(LIBS):
	@make -C ./lib/libft
	@make -C ./lib/libpf

clean:
	@rm -f $(OBJS)
	@printf $(CYAN)"[INFO]	$(NAME) cleaned\n"$(RESET)

fclean: clean
	@/bin/rm -f $(NAME) $(LINK)
	@/bin/rm -f ./*.so

re: fclean all

framework:
ifeq ($(SHELL_NAME), Darwin)
	@mkdir -p ~/Library/Frameworks
ifeq ("$(wildcard ~/Library/Frameworks/SDL2.framework)","")
	@cp -Rf $(SDL_DIR)/SDL2.framework ~/Library/Frameworks/
	@cp -Rf $(SDL_DIR)/SDL2_ttf.framework ~/Library/Frameworks/
	@printf $(CYAN)"[INFO]	Moving Frameworks\n"$(RESET)
else
	@printf $(CYAN)"[INFO]	Frameworks Exists\n"$(RESET)
endif
endif

.PHONY: clean, all, fclean, re
