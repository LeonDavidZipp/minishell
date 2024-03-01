# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/04 10:54:39 by lzipp             #+#    #+#              #
#    Updated: 2024/03/01 14:45:23 by lzipp            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := cc
CFLAGS := -Wextra -Wall -Werror
LDFLAGS := -lreadline

OBJDIR = ./obj
SOURCES := $(addprefix src/, \
build_ast/build_ast.c \
build_ast/ast_helpers.c \
builtin_cmds/cd.c \
builtin_cmds/echo.c \
builtin_cmds/env.c \
builtin_cmds/exit.c \
builtin_cmds/export.c \
builtin_cmds/pwd.c \
builtin_cmds/unset.c \
env_vars/env_vars.c \
env_vars/env_vars_2.c \
env_vars/init_environ.c \
lexer/check_tokens_valid.c \
lexer/join_tokens.c \
lexer/lexer.c \
lexer/token_type.c \
lexer/tokenize_helpers.c \
lexer/tokenize.c \
other_cmds/utils.c \
parsing/check_input.c \
parsing/expand_helpers.c \
parsing/expand_simple.c \
parsing/expand.c \
parsing/split_helpers.c \
parsing/split_helpers2.c \
parsing/split.c \
signals/signal_handler.c \
main.c)

OBJDIR = ./obj
OBJECTS := $(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))

LIBFT := lib/libft

all: $(NAME)

$(NAME): $(OBJECTS)
	make -C $(LIBFT)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME) -L $(LIBFT) -lft

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT) clean
	rm -rf $(OBJDIR)

fclean: clean
	make -C $(LIBFT) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
