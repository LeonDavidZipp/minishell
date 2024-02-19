# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lzipp <lzipp@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/04 10:54:39 by lzipp             #+#    #+#              #
#    Updated: 2024/02/16 17:08:08 by lzipp            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := cc
CFLAGS := -Wextra -Wall -Werror
LDFLAGS := -lreadline

SOURCES := $(addprefix src/, \
main.c \
signal_handler.c \
new_node.c)

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
