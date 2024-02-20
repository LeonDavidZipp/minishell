/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 10:58:02 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/20 12:21:23 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <termios.h>
# include <term.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>

# define PROMPT "\033[0;36mchl	→	\033[0m"

typedef enum e_token
{
	SEMICOLON,
	ARG,
	FLAG,
	BUILTIN_CMD,
	OTHER_CMD,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	PIPE,
	AND,
	OR,
	REDIR_OUT,
	REDIR_IN,
	REDIR_APPEND,
	REDIR_INPUT,
	HEREDOC,
	ENV_VAR,
	WILDCARD
}			t_token;

typedef struct s_input
{
	char	*content;
	int		type;
}			t_input;

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}					t_env_var;

typedef struct s_treenode
{
	char				*content;
	int					type;
	struct s_treenode	*left;
	struct s_treenode	*right;
}					t_treenode;

typedef struct s_app_data
{
	t_env_var	*env_vars;
	int			last_exit_code;
	char		*input;
}				t_app_data;

// signal handling
void		signal_handler(void);

// environment variables
char		**split_environ(char *environ);
t_env_var	*init_environ(char **environ);
t_env_var	*new_env_var(char *key, char *value);
void		update_env_vars(char *key, char *value, t_env_var **env_vars);
t_env_var	*copy_env_vars(t_env_var *env_vars);
void		free_env_vars(t_env_var *env_var);

// parsing && input handling
int			check_input(char *str);
char		**tokenize(char *input);
char		*add_spaces(char *input);
int			is_operator(char c, char d);
void		handle_quotes_brackets(char c, bool *in_quote, bool *in_bracket);

// expansion
char		*in_string_expansion(char *input, t_app_data *app);
char		**expand_wildcard(char *input);
char		*expand_var(char *input, t_env_var *env);
char		*expand_exit_code(int last_exit_code);

#endif
