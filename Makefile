# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/04 10:54:39 by lzipp             #+#    #+#              #
#    Updated: 2024/02/12 17:04:20 by cgerling         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := cc
CFLAGS := -Wextra -Wall -Werror
LDFLAGS := -lreadline

SOURCES := $(addprefix src/, \
main.c \
signal_handler.c)
OBJECTS := $(SOURCES:.c=.o)

LIBFT := lib/libft
LIBFT_LIB := $(LIBFT)/libft.a

all: $(NAME)

$(LIBFT_LIB):
	make -C $(LIBFT)

$(NAME): $(LIBFT_LIB) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME)

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
