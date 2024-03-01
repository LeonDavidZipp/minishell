/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 10:58:02 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 05:15:40 by lzipp            ###   ########.fr       */
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

# define LEXER_ERR "Error: Failed to tokenize input\n"
# define PARSER_ERR "Error: Failed to parse input\n"
# define EXEC_ERR "Error: Failed to execute command\n"
# define FORK_ERR "Error: Failed to fork process\n"
# define PIPE_ERR "Error: Failed to create pipe\n"
# define REDIR_ERR "Error: Failed to redirect\n"
# define CMD_NOT_FOUND "Error: Command not found\n"
# define CMD_NOT_EXEC "Error: Command not executable\n"
# define CMD_NOT_DIR "Error: Command is a directory\n"
# define CMD_NOT_FILE "Error: Command is not a file\n"
# define CMD_NOT_PERM "Error: Permission denied\n"
# define CMD_NOT_PIPE "Error: Pipe failed\n"
# define CMD_NOT_CLOSE "Error: Failed to close file descriptor\n"
# define CMD_NOT_DUP "Error: Failed to duplicate file descriptor\n"
# define CMD_NOT_WAIT "Error: Failed to wait for child process\n"
# define CMD_NOT_SIGNAL "Error: Failed to handle signal\n"
# define CMD_NOT_ENV "Error: Failed to handle envpment variable\n"
# define CMD_NOT_MALLOC "Error: Failed to allocate memory\n"
# define CMD_NOT_EXIT "Error: Failed to exit\n"

typedef enum e_tokentype
{
	// FLAG,
	// SINGLE_QUOTE,
	// DOUBLE_QUOTE,
	PIPE,
	AND,
	OR,
	REDIR_OUT, // >
	REDIR_IN, // <
	REDIR_APPEND, // >>
	HEREDOC, // <<
	WILDCARD,
	BUILTIN_CMD,
	OTHER_CMD,
	ARG
}			t_tokentype;

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}			t_env_var;

typedef struct s_token
{
	char			*content;
	t_tokentype		type;
	struct s_token	*next;
}			t_token;

typedef struct s_treenode
{
	char				*cmd;
	char				*args;
	struct s_treenode	*left;
	struct s_treenode	*right;
}			t_treenode;

typedef struct s_app_data
{
	t_env_var	*env_vars;
	int			last_exit_code;
	char		*input;
	t_token		*tokens;
}			t_app_data;

// signal handling
void		signal_handler(void);

// built-in commands
void		builtin_cd(char *path);
void		builtin_echo(char *str);
void		builtin_env(t_env_var **env_vars);
void		builtin_exit(int exit_code);
void		builtin_export(t_env_var **env_vars, char *var_string);

// enironment variables
t_env_var	*init_envp(char **envp);
t_env_var	*new_env_var(char *key, char *value);
t_env_var	*copy_env_vars(t_env_var *env_vars);
void		update_env_vars(char *key, char *value, t_env_var **env_vars);
void		free_env_vars(t_env_var *env_var);
char		**split_envp(char *envp);
char		**split_path(char *path);
char		*get_path(t_env_var *env_vars);

// parsing && input handling
int			is_space(char c);
int			check_input(char *str);
char		**split(char *input);
char		*add_spaces(char *input);
int			is_operator(char c, char d);
void		quotes_brackets(char c, bool *s_quote, bool *d_quote,
				bool *in_bracket);

// tokenization
t_token		*tokenize(t_app_data *app);
t_token		*join_arg_tokens(t_token *tokens);
t_token		*join_after_echo(t_token *tokens);
void		free_tokens(t_token *token);
t_tokentype	token_type(char *content, char *path);

// abstract syntax tree
t_treenode	*build_ast(t_treenode *lin_tree);
t_treenode	*combine_cmds_args(t_token *tokens);
void		free_treenodes(t_treenode *node);
void		debug_printtree(t_treenode *root, int tabs);
bool		check_nodes_valid(t_treenode *nodes);
bool		node_is_operator(char *cmd);
int			priority(char *cmd);

// lexer
void		lexer(t_app_data *app_data);

// expansion
char		*in_string_expansion(char *input, t_app_data *app);
int			get_new_size(char *input, int last_exit_code);
bool		match(char *pattern, char *string);
char		**expand_wildcard(char *input);
char		*expand_var(char *input);
char		*expand_exit_code(int last_exit_code);
bool		match(char *pattern, char *string);
int			get_new_size(char *input, int last_exit_code);

#endif