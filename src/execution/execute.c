/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/27 12:10:12 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// memory leaks!
// norminette!!!!!!!
// need to protect dup2 and dup!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// need to expand right before executing each command
// exit code is different for cat /dev/urandom | > out to bash
// exit minishell when too many open fds?!
// checken ob wir echo $_ expansion in minishell machen müssen

static int	execute_builtin(t_treenode *ast, t_app_data *app, t_pid_list **pid_list);
static int	execute_execve(t_treenode *ast, t_app_data *app, t_pid_list **pid_list);
void		exec_cmds(t_treenode *ast, t_app_data *app, t_pid_list **pid_list);
int			setup_redir(t_treenode *node, t_app_data *app);
int			setup_fd(t_treenode *node, t_app_data *app, int *ret);
char		*find_path(char *command, char **envp);
void		wait_and_free(t_app_data *app, t_pid_list **pid_list);
t_treenode	*find_cmd_node(t_treenode *node);
int			handle_heredoc(t_treenode *node, t_app_data *app);
int			is_builtin(char *cmd);
int			is_redir(t_tokentype type);

void	set_error_vars(t_treenode *node, char *err)
{
	node->err_val = errno;
	node->err = err;
}

int	execute(t_app_data *app, t_treenode *ast)
{
	t_pid_list	*pid_list;
	int		ret;

	pid_list = NULL;
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

	(void)app;
	g_exit_signal = 1;
	tmp = *pid_list;
	while (tmp)
	{
		waitpid(tmp->pid, &status, 0);
		if (WIFEXITED(status))
			app->last_exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			app->last_exit_code = 128 + WTERMSIG(status);
		}
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
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
		if (is_builtin(ast->cmd))
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

	g_exit_signal = 2;
	should_expand = true;
	if (delimiter[0] == '\'' || delimiter[0] == '"')
	{
		new_del = remove_quotes(delimiter);
		if (!new_del)
			return (1);
		should_expand = false;
	}
	else
		new_del = delimiter;
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
			expanded = expand(line, app->last_exit_code, app->env_vars, 1);
		else
			expanded = ft_strdup(line);
		if (!expanded)
		{
			free(line);
			return (1);
		}
		write(write_fd, expanded, ft_strlen(expanded));
		free(expanded);
		free(line);
		write(0, "> ", 2);
		line = get_next_line(0);
	}
	if (line)
		g_exit_signal = 0;
	free(line);
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
			printf("%s: pipe error: %s\n", NAME, strerror(errno));
			exit(1); // if we exit here, ret can be removed
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
			return (set_error_vars(node, "heredoc pipe error"), 1);
		cmd_node = find_cmd_node(node->left);
		if (cmd_node->cmd_type != CMD)
			set_error_vars(node, "heredoc pipe error");
		else
			set_error_vars(cmd_node, "heredoc pipe error");
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

void	set_err(t_treenode *node, char *err)
{
	t_treenode	*cmd_node;

	if (!node->left)
	{
		set_error_vars(node, err);
		return ;
	}
	cmd_node = find_cmd_node(node->left);
	if (cmd_node->cmd_type != CMD)
		set_error_vars(node, err);
	else
		set_error_vars(cmd_node, err);
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
			cmd_node->out_fd = fd;
			cmd_node->out_type = 1;
		}
		return ;
	}
	if (cmd_node && (cmd_node->in_type == 1 || cmd_node->in_type == 2))
		close(fd);
	else if (cmd_node)
	{
		cmd_node->in_fd = fd;
		cmd_node->in_type = 1;
	}
}

int	setup_redir(t_treenode *node, t_app_data *app)
{
	int			tmp_fd;
	char		*tmp;

	tmp = expand_and_remove(node->args, app->last_exit_code, app->env_vars);
	if (node->cmd_type == REDIR_IN)
	{
		tmp_fd = open(tmp, O_RDONLY);
		if (tmp_fd == -1)
			return (set_err(node, tmp), 1);
		else
			set_fd(node, tmp_fd, 2);
	}
	else if (node->cmd_type == HEREDOC && g_exit_signal != 2)
		return (handle_heredoc(node, app));
	else if (node->cmd_type == REDIR_OUT || node->cmd_type == REDIR_APPEND)
	{
		if (node->cmd_type == REDIR_OUT)
			tmp_fd = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			tmp_fd = open(tmp, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (tmp_fd == -1)
			return (set_err(node, tmp), 1);
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
	int		fd;

	
	cmd_node = ft_join_in_place(expand_and_remove(ast->cmd, app->last_exit_code, app->env_vars), " ");
	if (!cmd_node)
		return (1);
	if (ast->args)
	{
		tmp = ft_join_in_place(expand_and_remove(cmd_node, app->last_exit_code, app->env_vars), ast->args);
		if (!tmp)
			return (free(cmd_node), 1);
		arg_arr = ft_split(tmp, ' ');
		free(tmp);
	}
	else
		arg_arr = ft_split(cmd_node, ' ');
	if (!arg_arr)
		return 1;
	if (ast->err_val != 0)
	{
		ft_fprintf(2, "%s: %s: %s\n", NAME, ast->err, strerror(ast->err_val));
		return 1;
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
		execve(find_path(ast->cmd, app->env_vars), arg_arr, app->env_vars);
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
	return 0;
}

static int	execute_cmd(char *cmd, char *args, char *ast_args, t_app_data *app);

static int	execute_builtin(t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
{
	int		stdin_fd;
	int		stdout_fd;
	int		exit_code;
	int		pid;
	char	*cmd;
	char	*args;

	cmd = expand_and_remove(ast->cmd, app->last_exit_code, app->env_vars);
	if (!cmd)
		return (1);
	if (ast->args)
	{
		args = expand_and_remove(ast->args, app->last_exit_code, app->env_vars);
		if (!args)
			return (free(cmd), 1);
	}
	else
		args = NULL;
	exit_code = 0;
	if (ast->err_val != 0)
	{
		ft_fprintf(2,"%s: %s: %s\n", NAME, ast->err, strerror(ast->err_val));
		return (1);
	}
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

static int	execute_cmd(char *cmd, char *args, char *ast_args, t_app_data *app)
{
	int	exit_code;

	exit_code = 0;
	if (ft_strcmp(cmd, "cd") == 0)
		exit_code = builtin_cd(args);
	else if (ft_strcmp(cmd, "pwd") == 0)
		exit_code = builtin_pwd(args);
	else if (ft_strcmp(cmd, "echo") == 0)
		exit_code = builtin_echo(args, STDOUT_FILENO);
	else if (ft_strcmp(cmd, "env") == 0)
		exit_code = builtin_env(args, &app->env_vars);
	else if (ft_strcmp(cmd, "exit") == 0)
		builtin_exit(app, args);
	else if (ft_strcmp(cmd, "export") == 0)
		exit_code = builtin_export(ast_args, &app->env_vars, STDOUT_FILENO);
	else if (ft_strcmp(cmd, "unset") == 0)
		exit_code = builtin_unset(args, app->env_vars);
	else
		exit_code = 127;
	return (exit_code);
}
