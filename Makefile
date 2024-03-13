# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/01 20:30:26 by lzipp             #+#    #+#              #
#    Updated: 2024/03/13 14:54:01 by lzipp            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := cc
CFLAGS := -Wextra -Wall -Werror
LDFLAGS := -lreadline
# CFLAGS := -Wextra -Wall -Werror -fsanitize=address -fsanitize=undefined
# LDFLAGS := -lreadline -fsanitize=address -fsanitize=undefined


OBJDIR = ./obj
SOURCES := $(addprefix src/, \
build_ast/build_ast.c \
build_ast/ast_helpers.c \
build_ast/combine_cmds_args.c \
builtin_cmds/cd.c \
builtin_cmds/echo.c \
builtin_cmds/env.c \
builtin_cmds/exit.c \
builtin_cmds/export.c \
builtin_cmds/pwd.c \
builtin_cmds/unset.c \
env_vars/env_vars.c \
env_vars/split_multiple_envp.c \
execution/execute_helpers.c \
execution/execute.c \
lexer/check_tokens_valid.c \
lexer/join_tokens.c \
lexer/lexer.c \
lexer/switch_tokens_for_redir.c \
lexer/token_type.c \
lexer/tokenize.c \
parsing/check_input.c \
parsing/expand_helpers.c \
parsing/expand.c \
parsing/expand2.c \
parsing/mix_helpers.c \
parsing/need_name.c \
parsing/split_helpers.c \
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
