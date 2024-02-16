# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/04 10:54:39 by lzipp             #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2024/02/12 18:22:02 by lzipp            ###   ########.fr        #
=======
#    Updated: 2024/02/14 17:25:32 by lzipp            ###   ########.fr        #
>>>>>>> f46fc8d579603d9ab9cfb31c22fdf4c8d5e522a8
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
