# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/13 16:51:33 by nneronin          #+#    #+#              #
#    Updated: 2021/08/14 13:06:11 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =			test_malloc

CC =			gcc

FLAGS =			#-Wall -Werror -Wextra

FT_MALLOC =		../libft_malloc.so

LIBRARIES =		-L.. -lft_malloc

HEADERS =		-I ../include -I ../lib/libft -I ../lib/libpf

SRC_DIR =		./

OBJ_DIR_NAME =	obj
OBJ_DIR =		./obj/

FILENAMES =		main

OBJ_PATHS :=	$(addsuffix .o,$(FILENAMES))
OBJ_PATHS :=	$(addprefix $(OBJ_DIR),$(OBJ_PATHS))

all: $(NAME)

$(NAME): $(FT_MALLOC) $(OBJ_PATHS)
	$(CC) $(FLAGS) $(HEADERS) $(LIBRARIES) $(OBJ_PATHS) -o $(NAME)

$(FT_MALLOC):
	(cd .. && make re)

$(OBJ_PATHS): $(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@/bin/mkdir -p $(OBJ_DIR)
	$(CC) -c $(FLAGS) $(HEADERS) $< -o $@

clean:
	-/bin/rm -f $(OBJ_PATHS)
	/usr/bin/find . -name "$(OBJ_DIR_NAME)" -maxdepth 1 -type d -empty -delete

fclean: clean
	-/bin/rm -f $(NAME)

re: fclean all
	(cd .. && make re)
