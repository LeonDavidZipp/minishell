# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/04 10:54:39 by lzipp             #+#    #+#              #
#    Updated: 2024/02/23 15:06:30 by lzipp            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := cc
CFLAGS := -Wextra -Wall -Werror
LDFLAGS := -lreadline

SOURCES := $(addprefix src/, \
bin_tree/new_node.c \
builtin_cmds/cd.c \
builtin_cmds/echo.c \
builtin_cmds/env.c \
builtin_cmds/exit.c \
builtin_cmds/export.c \
builtin_cmds/pwd.c \
env_vars/env_vars.c \
env_vars/init_environ.c \
main.c \
signals/signal_handler.c)
# parsing/check_input.c \
# parsing/expand_simple.c \
# parsing/expand.c \
# parsing/tokenize.c \

OBJECTS := $(SOURCES:.c=.o)

LIBFT := lib/libft

all: $(NAME)

$(NAME): $(OBJECTS)
	make -C $(LIBFT)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME) -L $(LIBFT) -lft

clean:
	make -C $(LIBFT) clean
	rm -f $(OBJECTS)
	
fclean: clean
	make -C $(LIBFT) fclean
	rm -f $(NAME)

re: fclean all

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re
