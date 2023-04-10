# -*- coding: utf-8 -*-
# vim: ts=4 sw=4 tw=80 et ai si
#
# Created: 08/04/2023 Niklas Neronin
# Updated: 08/04/2023 Niklas Neronin

RED 	:= "\e[0;31m"
GREEN	:= "\e[0;32m"
YELLOW	:= "\e[0;33m"
CYAN	:= "\e[0;36m"
RESET	:= "\e[0m"

LIBS =	./lib/libft/libft.a\
		./lib/libpf/libpf.a

IDIR =	-I ./include \
		-I ./lib/libft \
		-I ./lib/libpf

RAW =	malloc.c\
		calloc.c\
		realloc.c\
		free.c\
		zone_utils.c\
		ft_malloc.c\
		ft_mempurge.c\
		ft_memfind.c\

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libdm_$(HOSTTYPE).so
CDIR = srcs
ODIR = objs
SRCS = $(addprefix $(CDIR)/, $(RAW))
OBJS = $(addprefix $(ODIR)/, $(RAW:.c=.o))
CFLAGS = -Wall -Wextra -MMD

all: $(NAME)
	@printf ${GREEN}"\e[J> $(NAME) ready!\n"${RESET}

$(ODIR):
	@mkdir -p $(ODIR)

$(ODIR)/%.o: $(CDIR)/%.c
	@printf $(YELLOW)"\e[JCompiling $<\n\e[F"$(RESET)
	@gcc $(CFLAGS) -fPIC -c $< -o $@ $(IDIR) $(LIBS)

$(NAME): $(ODIR) $(OBJS)
	@gcc $(OBJS) $(IDIR) $(CFLAGS) -shared -o $(NAME)

clean:
	@printf ${CYAN}"[INFO] Removing object files.\n"${RESET}
	@rm -rf $(ODIR)

fclean: clean
	@printf ${CYAN}"[INFO] Removing $(NAME)\n"${RESET}
	@rm -f $(NAME)

re: fclean all

.PHONY: clean, all, fclean, re
