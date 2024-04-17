/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 19:52:57 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/17 16:23:52 by cgerling         ###   ########.fr       */
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

# define NAME "babash"
# define PROMPT "\033[1;36mbabash \033[1;32m→  \033[0m"
# define SYN_MSG "syntax error near unexpected token"
# define INVALID_ID "not a valid identifier"
# define INVALID_OP "invalid option"
# define DOT_MSG ": .: filename argument required\n"
# define DOT_MSG2 ".: usage: . filename [arguments]\n"
# define UNSET_USG "unset: usage: unset [-f] [-v] [name ...]\n"
# define EXPORT_USG "export: usage: export [-nf] "
# define EXPORT_USG2 "[name[=value] ...] or export -p\n"

extern int	g_exit_signal;

typedef enum e_tokentype
{
	FIRST,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	PIPE,
	AND,
	OR,
	REDIR_OUT,
	REDIR_IN,
	REDIR_APPEND,
	HEREDOC,
	CMD,
	ARG
}			t_tokentype;

// REDIR_OUT, // >
// REDIR_IN, // <
// REDIR_APPEND, // >>
// HEREDOC, // <<

typedef struct s_token
{
	char				*content;
	t_tokentype			type;
	struct s_token		*next;
}			t_token;

typedef struct s_treenode
{
	char				*cmd;
	t_tokentype			cmd_type;
	char				*args;
	int					bracket_lvl;
	int					in_fd;
	int					out_fd;
	int					err_val;
	int					in_type;
	int					out_type;
	char				*err;
	bool				pipe;
	struct s_treenode	*left;
	struct s_treenode	*right;
}			t_treenode;

typedef struct s_app_data
{
	char				**env_vars;
	int					last_exit_code;
	char				*input;
	int					noninteractive;
}			t_app_data;

typedef struct s_pid_list
{
	pid_t				pid;
	struct s_pid_list	*next;
}			t_pid_list;

typedef struct s_envvar
{
	char				*key;
	char				*value;
	bool				includes_equal;
	bool				includes_plus;
}			t_envvar;

typedef struct s_expand
{
	char				*input;
	char				**output;
	int					*i;
	bool				*quotes;
	char				**env_vars;
	int					exit_code;
	int					*flags;
}			t_expand;

typedef struct s_builtin
{
	char				*cmd;
	char				*args;
	int 				exit_code;
	int					stdin_fd;
	int					stdout_fd;
	pid_t				pid;
}			t_builtin;

typedef struct s_execve
{
	pid_t				pid;
	char				**arg_arr;
	char				*cmd_node;
	char				*tmp;
	char				*temp;
	int					i;
	int					flags[2];
	bool				flag;
	struct stat			path_stat;
}			t_execve;

// signal handling
void		signal_handler(void);

// temporary
int	execute_builtin(t_treenode *ast, t_app_data *app, t_pid_list **pid_list);
int	execute_execve(t_treenode *ast, t_app_data *app, t_pid_list **pid_list);
int			setup_fd(t_treenode *node, t_app_data *app, int *ret);
char		*find_path(char *command, char **envp, bool *flag);
void		wait_and_free(t_app_data *app, t_pid_list **pid_list);
t_treenode	*find_cmd_node(t_treenode *node);
int			handle_heredoc(t_treenode *node, t_app_data *app);
int		is_builtin(char *cmd, int exit_code, char **env_vars);
int			is_redir(t_tokentype type);
int		read_input(char *delimiter, int write_fd, t_app_data *app);
void	set_error_vars(t_treenode *node, char *err, int val);
void	set_err(t_treenode *node, char *err, int val);
int	ambigious_redirect(char *str);
int	add_to_pid_list(pid_t pid, t_pid_list **pidlist);
void	set_fd(t_treenode *node, int fd, int flag);

int	is_hidden_command(char *command, char **env_vars);
int	exec_hidden_command(char *hidden_command, char **args, t_app_data *app, t_pid_list **pid_list);
int	execute_cmd(char *cmd, char *args, char *ast_args, t_app_data *app);

// execution
void		exec_cmds(t_treenode *ast, t_app_data *app,
				t_pid_list **pid_list);
int			setup_redir(t_treenode *node, t_app_data *app);
void		close_fds_loop(void);
void		handle_error(t_treenode *ast);
int			handle_fds_dup2(t_treenode *ast);

// built-in commands
int			builtin_cd(char *path, char ***env_vars, int *last_exit_code);
int			builtin_pwd(char *args, int fd);
int			builtin_env(char *var_string, char ***env_vars);
int			builtin_echo(char *args, int out_fd, t_app_data **app);
int			builtin_exit(t_app_data *app, char *args);
int			builtin_unset(char *keys, char **env_vars);
int			builtin_export(char *var_string, t_app_data **app_data, int fd);

// environment variables
char		**init_envp(char **env_vars);
char		**update_env_vars(t_envvar **var, int *exit_code, char **env_vars);
void		unset_env_var(char *key, char ***env_vars);
int			unset_multiple_env_vars(char *keys_string, char ***env_vars);
t_envvar	**split_env_vars(char *envp, t_app_data **app_data);
t_envvar	*split_env_var(char *envp);
void		free_vars(t_envvar **vars);
int			var_name_valid(char *key, bool includes_equal);
void		handle_non_zero(char **key, int *exit_code);
t_envvar	*fill_result(t_envvar *result, char *envp);
t_envvar	*fill_result2(t_envvar *result, char *envp, int len);

// parsing && input handling
int			is_space(char c);
int			check_input(char *str);
char		**split(char *input);
char		*add_spaces(char *input);
int			is_operator(char c, char d);
char		*remove_quotes(char *str);

// redirection switching
t_token		*switch_tokens_for_redir(t_token *tokens);
bool		rearrange_condition(t_token *current);
bool		before_first_condition(t_token *current);
void		rearrange_first_element(t_token **tokens, t_token **current,
				t_token **before_first);
void		rearrange_following_element(t_token **current,
				t_token **before_first);

// heredoc switching
t_treenode	*switch_heredocs(t_treenode *lintree);

// tokenization
t_token		*new_token(char *content, t_token *prev);
t_token		*tokenize(char *input, int *exit_code, int *err_loc);
t_token		*join_arg_tokens(t_token *tokens);
t_token		*remove_after_cd(t_token *tokens);
t_tokentype	token_type(char *content, t_tokentype prev_type);
int			check_tokens_valid(t_token *tokens, int *err_loc);
void		free_tokens(t_token *token);

// abstract syntax tree
t_treenode	*build_ast(t_treenode *ast, t_treenode *lin_tree, int bracket_lvl);
t_treenode	*combine_cmds_args(t_token *tokens);
t_treenode	*insert_node(t_treenode *root, t_treenode *node);
t_treenode	*new_treenode(char *cmd, char *args,
				t_tokentype cmd_type, int bracket_lvl);
void		free_treenodes(t_treenode *node);
void		debug_printtree(t_treenode *root, int tabs);
bool		node_is_operator(t_tokentype type);
int			priority(char *cmd, int bracket_lvl);

// lexer
int			lexer(t_app_data *app_data);
void		prepare_heredoc_check(char *input, int err_loc);

// execution
int			execute(t_app_data *app, t_treenode *ast);
int			execute_cmd(char *cmd, char *args, char *ast_args,
				t_app_data *app);
int			is_hidden_command(char *command, char **env_vars);
int			exec_hidden_command(char *hidden_command, char **args,
				t_app_data *app, t_pid_list **pid_list);
int			add_to_pid_list(pid_t pid, t_pid_list **pidlist);
char		*find_path(char *command, char **envp, bool *flag);
char		*find_path_no_err(char *command, char **envp);
int			is_builtin(char *cmd, int exit_code, char **env_vars);
int			is_builtin_no_expand(char *cmd);
int			hidden_builtin(char *hidden_command, t_app_data *app);
int			hidden_execve(char *hidden_command, t_app_data *app,
				t_pid_list **pid_list);

// expansion
char		*expand(char *input, int exit_code, char **env_vars, int *flags);
int			handle_dollar(t_expand *data);
void		handle_quotes(char c, bool *s_quote, bool *d_quote);
int			get_new_size(char *input, int exit_code, char **env_vars,
				int *flags);
bool		match(char *pattern, char *string, bool s_quote, bool d_quote);
char		*get_pattern(char *input, int *i, int *position, int *start);
char		*remove_quotes_in_place(char *str);
char		*expand_and_remove(char *str, int exit_code, char **env_vars,
				int flag);
char		*expand_and_remove_in_place(char *str, int exit_code,
				char **env_vars, int flag);
char		*ft_getenv(char *name, char **env_vars);
int			is_valid_dollar(char *input, int i, bool *quotes);
int			wildcard_size(char *input, int *i);
int			empty_entry(char *entry);
int			calc_wildcard_size(DIR *dir, char *pattern, int position);
int			process_dir_entries(DIR *dir, char *pattern, char **output,
				int *i);
int			handle_wildcard(char *input, char **output, int *i);

#endif
