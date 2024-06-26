# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/01 20:30:26 by lzipp             #+#    #+#              #
#    Updated: 2024/04/28 14:52:14 by lzipp            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := babash

CC := cc
CFLAGS := -Wextra -Wall -Werror
LDFLAGS := -lreadline

OBJDIR = ./obj
SOURCES := $(addprefix src/, \
build_ast/build_ast.c \
build_ast/ast_helpers.c \
build_ast/combine_cmds_args.c \
builtin_cmds/cd.c \
builtin_cmds/echo.c \
builtin_cmds/env.c \
builtin_cmds/exit.c \
builtin_cmds/exit_helpers.c \
builtin_cmds/export.c \
builtin_cmds/pwd.c \
builtin_cmds/unset.c \
env_vars/env_var_helpers.c \
env_vars/env_var_helpers2.c \
env_vars/env_vars.c \
env_vars/split_multiple_envp.c \
execution/check_types.c \
execution/execute_helpers.c \
execution/execute.c \
execution/hidden_execve.c \
execution/hidden_execve_helpers.c \
execution/setup_fds.c \
execution/exec_builtin.c \
execution/exec_execve.c \
execution/exec_execve_helpers.c \
execution/heredoc.c \
execution/heredoc_helpers.c \
execution/setup_helpers.c \
execution/execute_helpers2.c \
lexer/check_tokens_valid.c \
lexer/heredoc.c \
lexer/join_arg_tokens.c \
lexer/remove_after_cd.c \
lexer/lexer.c \
lexer/switch_heredocs.c \
lexer/switch_tokens_for_redir.c \
lexer/switch_tokens_for_redir_2.c \
lexer/token_type.c \
lexer/tokenize.c \
parsing/check_input.c \
parsing/check_types.c \
parsing/expand_helpers.c \
parsing/expand_helpers2.c \
parsing/expand.c \
parsing/expand2.c \
parsing/mix_helpers.c \
parsing/expand_and_remove.c \
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
