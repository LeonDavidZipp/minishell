/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/16 15:43:06 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// memory leaks!
// norminette!!!!!!!
// need to protect dup2 and dup!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// exit code is different for cat /dev/urandom | > out to bash
// ctrl c sets exit code to 1 used on an empty line

// heredoc ctrl d cant't exit on a line where something is written

static int	execute_builtin(t_treenode *ast, t_app_data *app, t_pid_list **pid_list);
static int	execute_execve(t_treenode *ast, t_app_data *app, t_pid_list **pid_list);
void		exec_cmds(t_treenode *ast, t_app_data *app, t_pid_list **pid_list);
int			setup_redir(t_treenode *node, t_app_data *app);
int			setup_fd(t_treenode *node, t_app_data *app, int *ret);
char		*find_path(char *command, char **envp, bool *flag);
void		wait_and_free(t_app_data *app, t_pid_list **pid_list);
t_treenode	*find_cmd_node(t_treenode *node);
int			handle_heredoc(t_treenode *node, t_app_data *app);
int		is_builtin(char *cmd, int exit_code, char **env_vars);
int			is_redir(t_tokentype type);

int	is_hidden_command(char *command, char **env_vars);
int	exec_hidden_command(char *hidden_command, char **args, t_app_data *app, t_pid_list **pid_list);

void	set_error_vars(t_treenode *node, char *err, int val)
{
	node->err_val = val;
	node->err = err;
}

int	execute(t_app_data *app, t_treenode *ast)
{
	t_pid_list	*pid_list;
	int		ret;

	ret = 0;
	pid_list = NULL;
	ret = 0;
	setup_fd(ast, app, &ret);
	if (ret == 2 || g_exit_signal == 2)
	{
		app->last_exit_code = 1;
		return (1);
	}
	// debug_printtree(ast, 0);
	exec_cmds(ast, app, &pid_list);
	wait_and_free(app, &pid_list);
	return (0);
}

void	wait_and_free(t_app_data *app, t_pid_list **pid_list)
{
	t_pid_list	*tmp;
	t_pid_list	*next;
	int			status;
	int			exit_code;

	g_exit_signal = 1;
	tmp = *pid_list;
	exit_code = 0;
	while (tmp)
	{
		waitpid(tmp->pid, &status, 0);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			exit_code = 128 + WTERMSIG(status);
		}
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	if (app->last_exit_code == 0)
		app->last_exit_code = exit_code;
	*pid_list = NULL;
	g_exit_signal = 0;
}

int	check_for_errors(t_treenode *ast, int last_exit_code)
{
	if (ast->err_val != 0)
	{
		ft_fprintf(2, "%s: %s: %s\n", NAME, ast->err, strerror(ast->err_val));
		return (1);
	}
	if (last_exit_code != 0)
		return (last_exit_code);
	return (0);
}

void exec_cmds(t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
{
	if (ast == NULL)
		return;
	if (ast->left)
		exec_cmds(ast->left, app, pid_list);
	if (ast->cmd_type == CMD)
	{
		if (is_builtin(ast->cmd, app->last_exit_code, app->env_vars))
			app->last_exit_code = execute_builtin(ast, app, pid_list);
		else
			app->last_exit_code = execute_execve(ast, app, pid_list);
	}
	else if (ast->cmd_type == PIPE || is_redir(ast->cmd_type))
		app->last_exit_code = check_for_errors(ast, app->last_exit_code);
	else if (ast->cmd_type == AND || ast->cmd_type == OR)
	{
		wait_and_free(app, pid_list);
		if ((ast->cmd_type == AND && app->last_exit_code == 0)
			|| (ast->cmd_type == OR && app->last_exit_code != 0))
			exec_cmds(ast->right, app, pid_list);
	}
	if (ast->cmd_type != AND && ast->cmd_type != OR)
	{
		if (ast->right)
			exec_cmds(ast->right, app, pid_list);
	}
}

t_treenode *find_cmd_node(t_treenode *node)
{
	while (node->cmd_type != CMD)
	{
		if (node->right)
			node = node->right;
		else if (node->left)
			node = node->left;
		else
			break;
	}
	return (node);
}

int	read_input(char *delimiter, int write_fd, t_app_data *app)
{
	char	*line;
	char	*expanded;
	char	*tmp;
	char	*new_del;
	bool	should_expand;
	int		i;
	int		flags[2];

	i = 0;
	flags[0] = 1;
	flags[1] = 0;
	g_exit_signal = 2;
	should_expand = true;
	while (delimiter[i] != '\0')
	{
		if (delimiter[i] == '\'' || delimiter[i] == '"')
			should_expand = false;
		i++;
	}
	new_del = remove_quotes(delimiter);
	if (!new_del)
		return (1);
	write(0, "> ", 2);
	line = get_next_line(0);
	while (line != NULL)
	{
		tmp = ft_strtrim(line, "\n");
		if (ft_strncmp(tmp, new_del, ft_strlen(new_del)) == 0
			&& ft_strlen(tmp) == ft_strlen(new_del))
			break ;
		free(tmp);
		if (should_expand)
			expanded = expand(line, app->last_exit_code, app->env_vars, flags);
		else
			expanded = ft_strdup(line);
		if (!expanded)
			return (free(line), free(new_del), 1);
		write(write_fd, expanded, ft_strlen(expanded));
		free(expanded);
		free(line);
		write(0, "> ", 2);
		line = get_next_line(0);
	}
	if (line)
		g_exit_signal = 0;
	free(line);
	free(new_del);
	close(write_fd);
	return (0);
}

int	setup_fd(t_treenode *node, t_app_data *app, int *ret)
{
	int	pipe_fd[2];
	t_treenode *cmd_node;
	
	if (node->cmd_type == PIPE)
	{
		if (pipe(pipe_fd) == -1)
		{
			ft_fprintf(2, "%s: pipe error: %s\n", NAME, strerror(errno));
			*ret = 2;
			return (2);
		}
		else
		{
			cmd_node = find_cmd_node(node->left);
			if (cmd_node->cmd_type != CMD)
				close(pipe_fd[1]);
			else
			{
				cmd_node->out_fd = pipe_fd[1];
				cmd_node->pipe = true;
			}
			cmd_node = find_cmd_node(node->right);
			if (cmd_node->cmd_type != CMD)
				close(pipe_fd[0]);
			else
			{
				cmd_node->in_fd = pipe_fd[0];
				cmd_node->pipe = true;
			}
		}
	}
	if (is_redir(node->cmd_type))
		setup_redir(node, app);
	if (node->left)
		setup_fd(node->left, app, ret);
	if (node->right)
		setup_fd(node->right, app, ret);
	return (0);
}

int	handle_heredoc(t_treenode *node, t_app_data *app)
{
	int			tmp_fd;
	int			pipe_fd[2];
	t_treenode	*cmd_node;

	if (pipe(pipe_fd) == -1)
	{
		if (!node->left)
			return (set_error_vars(node, "heredoc pipe error", errno), 1);
		cmd_node = find_cmd_node(node->left);
		if (cmd_node->cmd_type != CMD)
			set_error_vars(node, "heredoc pipe error", errno);
		else
			set_error_vars(cmd_node, "heredoc pipe error", errno);
	}
	if (read_input(node->args, pipe_fd[1], app))
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	tmp_fd = pipe_fd[0];
	if (!node->left)
	{
		close(tmp_fd);
		return (1);
	}
	cmd_node = find_cmd_node(node->left);
	if (cmd_node->cmd_type != CMD)
		return (close(tmp_fd), 1);
	if (cmd_node->in_type == 1)
		close(tmp_fd);
	else
	{
		cmd_node->in_fd = tmp_fd;
		cmd_node->in_type = 2;
	}
	return (0);
}

void	set_err(t_treenode *node, char *err, int val)
{
	t_treenode	*cmd_node;

	if (!node->left)
	{
		set_error_vars(node, err, val);
		return ;
	}
	cmd_node = find_cmd_node(node->left);
	if (cmd_node->cmd_type != CMD)
		set_error_vars(node, err, val);
	else
		set_error_vars(cmd_node, err, val);
	return ;
}

void	set_fd(t_treenode *node, int fd, int flag)
{
	t_treenode	*cmd_node;
	
	if (!node->left)
	{
		close(fd);
		return ;
	}
	cmd_node = find_cmd_node(node->left);
	if (cmd_node && cmd_node->cmd_type != CMD)
		close(fd);
	if (cmd_node && flag == 1)
	{
		if (cmd_node->out_type == 1)
			close(fd);
		else
		{
			if (cmd_node->out_fd != STDOUT_FILENO)
				close(cmd_node->out_fd);
			cmd_node->out_fd = fd;
			cmd_node->out_type = 1;
		}
	}
	else
	{
		if (cmd_node->in_type == 1 || cmd_node->in_type == 2)
			close(fd);
		else
		{
			if (cmd_node->in_fd != STDIN_FILENO)
				close(cmd_node->in_fd);
			cmd_node->in_fd = fd;
			cmd_node->in_type = 1;
		}
	}
}

int	ambigious_redirect(char *str)
{
	int		i;
	bool	s_quote;
	bool	d_quote;

	i = 0;
	s_quote = false;
	d_quote = false;
	while (str[i])
	{
		handle_quotes(str[i], &s_quote, &d_quote);
		if (str[i] == ' ' && !s_quote && !d_quote)
			return (1);
		i++;
	}
	return (0);
}

int	setup_redir(t_treenode *node, t_app_data *app)
{
	int			tmp_fd;
	char		*tmp;

	int flags[2] = {0, 0};
	if (!node->args)
		return (1);
	char *tmp2 = expand(node->args, app->last_exit_code, app->env_vars, flags);
	if (!tmp2)
		return (1);
	if (ambigious_redirect(tmp2))
		return (set_err(node, ft_strdup(node->args), -1), free(tmp2), 1);
	tmp = remove_quotes_in_place(tmp2);
	if (node->cmd_type == REDIR_IN)
	{
		tmp_fd = open(tmp, O_RDONLY);
		if (tmp_fd == -1)
			return (set_err(node, tmp, errno), 1);
		else
			set_fd(node, tmp_fd, 2);
	}
	else if (node->cmd_type == HEREDOC && g_exit_signal != 2)
		return (free(tmp), handle_heredoc(node, app));
	else if (node->cmd_type == REDIR_OUT || node->cmd_type == REDIR_APPEND)
	{
		if (node->cmd_type == REDIR_OUT)
			tmp_fd = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			tmp_fd = open(tmp, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (tmp_fd == -1)
			return (set_err(node, tmp, errno), 1);
		else
			set_fd(node, tmp_fd, 1);
	}
	free(tmp);
	return (0);
}

int	add_to_pid_list(pid_t pid, t_pid_list **pidlist)
{
	t_pid_list	*new;
	t_pid_list	*tmp;
	
	new = (t_pid_list *)malloc(sizeof(t_pid_list));
	if (!new)
		return (1);
	new->pid = pid;
	new->next = NULL;
	if (*pidlist == NULL)
		*pidlist = new;
	else
	{
		tmp = *pidlist;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (0);
}

static int	execute_execve(t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
{
	pid_t	pid;
	char	**arg_arr;
	char	*cmd_node;
	char	*tmp;
	char	*temp;
	int		fd;
	int		i;
	bool	flag = false;

	if (ast->err_val != 0)
	{
		if (ast->err_val == -1)
			ft_fprintf(2, "%s: %s: ambiguous redirect\n", NAME, ast->err);
		else
			ft_fprintf(2, "%s: %s: %s\n", NAME, ast->err, strerror(ast->err_val));
		if (ast->in_fd != 0)
			close(ast->in_fd);
		if (ast->out_fd != 1)
			close(ast->out_fd);
		return (1);
	}
	cmd_node = ft_strjoin(ast->cmd, " ");
	if (!cmd_node)
		return (1);
	if (ast->args)
	{
		tmp = ft_strjoin(cmd_node, ast->args);
		if (!tmp)
			return (free(cmd_node), 1);
		int flags[2];
		flags[0] = 0;
		flags[1] = 0;
		temp = expand(tmp, app->last_exit_code, app->env_vars, flags);
		if (!temp)
			return (free(cmd_node), free(tmp), 1);
		free(tmp);
		tmp = expand(temp, app->last_exit_code, app->env_vars, flags);
		if (!tmp)
			return (free(cmd_node), free(temp), 1);
		arg_arr = split(tmp);
		free(tmp);
		free(temp);
	}
	else
		arg_arr = split(cmd_node);
	if (!arg_arr)
		return 1;
	i = 0;
	while (arg_arr[i])
	{
		temp = expand_and_remove(arg_arr[i], app->last_exit_code, app->env_vars, 0);
		if (!temp)
			return (free(cmd_node), ft_free_2d_arr((void **)arg_arr), 1);
		free(arg_arr[i]);
		arg_arr[i] = temp;
		i++;
	}
	if (is_hidden_command(arg_arr[0], app->env_vars) && ast->cmd[0] == '$')
	{
		if (is_hidden_command(arg_arr[0], app->env_vars) == 2 && !arg_arr[1])
			return (free(cmd_node), ft_free_2d_arr((void **)arg_arr), 0);
		else if (is_hidden_command(arg_arr[0], app->env_vars) == 2 && arg_arr[1])
		{
			int	exit_code = exec_hidden_command(arg_arr[1], arg_arr, app, pid_list);
			return (free(cmd_node), ft_free_2d_arr((void **)arg_arr), exit_code);
		}
		int	exit_code = exec_hidden_command(arg_arr[0], arg_arr, app, pid_list);
		return (free(cmd_node), ft_free_2d_arr((void **)arg_arr), exit_code);
	}
	pid = fork();
	if (pid == -1)
	{
		ft_fprintf(2, "%s: fork error: %s\n", NAME, strerror(errno));
		return 1;
	}
	if (pid == 0)
	{
		if (ast->in_fd != STDIN_FILENO)
		{
			dup2(ast->in_fd, STDIN_FILENO);
			close(ast->in_fd);
		}
		if (ast->out_fd != STDOUT_FILENO)
		{
			dup2(ast->out_fd, STDOUT_FILENO);
			close(ast->out_fd);
		}
		fd = 3;
		while (fd < FD_SETSIZE)
		{
			if (fd != STDIN_FILENO && fd != STDOUT_FILENO)
				close(fd);
			fd++;
		}
		struct stat path_stat;
		stat(arg_arr[0], &path_stat);
		if (S_ISDIR(path_stat.st_mode))
		{
			if (ft_strcmp(arg_arr[0], ".") == 0)
			{
				ft_fprintf(2, "%s%s%s", NAME, DOT_MSG, DOT_MSG2);
				exit(127);
			}
			if (ft_strcmp(arg_arr[0], "..") == 0)
			{
				ft_fprintf(2, "%s: ..: command not found\n", NAME);
				exit(127);
			}
			ft_fprintf(2, "%s: %s: is a directory\n", NAME, arg_arr[0]);
			exit(126);
		}
		if (access(arg_arr[0], X_OK) == 0)
			execve(arg_arr[0], arg_arr, app->env_vars);
		else
			execve(find_path(arg_arr[0], app->env_vars, &flag), arg_arr, app->env_vars);
		if (flag)
			exit(126);
		exit(127);
	}
	else
	{
		if (add_to_pid_list(pid, pid_list))
			return (free(cmd_node), ft_free_2d_arr((void **)arg_arr), 1);
		if (ast->in_fd != 0)
			close(ast->in_fd);
		if (ast->out_fd != 1)
			close(ast->out_fd);
	}
	free(cmd_node);
	ft_free_2d_arr((void **)arg_arr);
	return (0);
}

int	execute_cmd(char *cmd, char *args, char *ast_args, t_app_data *app);

static int	execute_builtin(t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
{
	int		stdin_fd;
	int		stdout_fd;
	int		exit_code;
	int		pid;
	char	*cmd;
	char	*args;

	if (ast->err_val != 0)
	{
		if (ast->err_val == -1)
			ft_fprintf(2,"%s: %s: ambiguous redirect\n", NAME, ast->err);
		else
			ft_fprintf(2,"%s: %s: %s\n", NAME, ast->err, strerror(ast->err_val));
		if (ast->in_fd != 0)
			close(ast->in_fd);
		if (ast->out_fd != 1)
			close(ast->out_fd);
		return (1);
	}
	cmd = expand_and_remove(ast->cmd, app->last_exit_code, app->env_vars, 0);
	if (!cmd)
		return (1);
	if (ast->args)
	{
		args = expand_and_remove(ast->args, app->last_exit_code, app->env_vars, 0);
		if (!args)
			return (free(cmd), 1);
	}
	else
		args = NULL;
	exit_code = 0;
	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (ast->in_fd != 0)
	{
		dup2(ast->in_fd, STDIN_FILENO);
		close(ast->in_fd);
	}
	if (ast->out_fd != 1)
	{
		dup2(ast->out_fd, STDOUT_FILENO);
		close(ast->out_fd);
	}
	if (ast->pipe == true)
	{
		pid = fork();
		if (pid == -1)
		{
			ft_fprintf(2, "%s: fork error: %s\n", NAME, strerror(errno));
			return (1);
		}
		if (pid == 0)
		{
			exit_code = execute_cmd(cmd, args, ast->args, app);
			int fd = 3;
			while (fd < FD_SETSIZE)
			{
				if (fd != STDIN_FILENO && fd != STDOUT_FILENO)
					close(fd);
				fd++;
			}
			exit(exit_code);
		}
		else
		{
			if (add_to_pid_list(pid, pid_list))
				return (1);
		}
	}
	else
		exit_code = execute_cmd(cmd, args, ast->args, app);
	dup2(stdin_fd, STDIN_FILENO);
	close(stdin_fd);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdout_fd);
	free(cmd);
	if (ast->args)
		free(args);
	return (exit_code);
}

int	execute_cmd(char *cmd, char *args, char *ast_args, t_app_data *app)
{
	int	exit_code;

	exit_code = 0;
	if (ft_strcmp(cmd, "cd") == 0)
		exit_code = builtin_cd(args, &app->env_vars, &app->last_exit_code);
	else if (ft_strcmp(cmd, "pwd") == 0)
		exit_code = builtin_pwd(args, STDOUT_FILENO);
	else if (ft_strcmp(cmd, "echo") == 0)
		exit_code = builtin_echo(ast_args, STDOUT_FILENO, &app);
	else if (ft_strcmp(cmd, "env") == 0)
		exit_code = builtin_env(args, &app->env_vars);
	else if (ft_strcmp(cmd, "exit") == 0)
		exit_code = builtin_exit(app, args);
	else if (ft_strcmp(cmd, "export") == 0)
		exit_code = builtin_export(ast_args, &app, STDOUT_FILENO);
	else if (ft_strcmp(cmd, "unset") == 0)
		exit_code = builtin_unset(args, app->env_vars);
	else
		exit_code = 127;
	return (exit_code);
}
