/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/12 19:08:58 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// problem with heredoc because in tree the last redirect is the first one encountered when setting up the fd's
// maybe need to change waiting for children, not sure?!
// memory leaks!
// norminette!!!!!!!

static int	execute_builtin(t_treenode *ast, t_app_data *app);
static int	execute_execve(t_treenode *ast, char **env_vars, t_pid_list **pid_list);
void		exec_cmds(t_treenode *ast, t_app_data *app, t_pid_list **pid_list);
void		setup_redir(t_treenode *node);
void		setup_fd(t_treenode *node);
char		*find_path(char *command, char **envp);

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

int	is_redir(t_tokentype type)
{
	if (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC)
		return (1);
	return (0);
}

void	execute(t_app_data *app, t_treenode *ast)
{
	t_pid_list	*pid_list;
	int			status;

	pid_list = NULL;
	(void)app;
	setup_fd(ast);
	// debug_printtree(ast, 0);
	exec_cmds(ast, app, &pid_list);
	t_pid_list	*tmp;
	tmp = pid_list;
	while (tmp)
	{
		waitpid(tmp->pid, &status, 0);
		if (WIFEXITED(status))
			app->last_exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			app->last_exit_code = 128 + WTERMSIG(status);
		tmp = tmp->next;
	}
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
			app->last_exit_code = execute_builtin(ast, app);
		else
			app->last_exit_code = execute_execve(ast, app->env_vars, pid_list);
	}
	else if (ast->cmd_type == AND || ast->cmd_type == OR)
	{
		t_pid_list	*tmp;
		int			status;
		tmp = *pid_list;
		while (tmp)
		{
			waitpid(tmp->pid, &status, 0);
			if (WIFEXITED(status))
				app->last_exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				app->last_exit_code = 128 + WTERMSIG(status);
			tmp = tmp->next;
		}
		if (ast->cmd_type == AND && app->last_exit_code == 0)
			exec_cmds(ast->right, app, pid_list);
		else if (ast->cmd_type == OR && app->last_exit_code != 0)
			exec_cmds(ast->right, app, pid_list);
	}
	if (ast->cmd_type != AND && ast->cmd_type != OR)
	{
		if (ast->right)
			exec_cmds(ast->right, app, pid_list);
	}
}

void	setup_fd(t_treenode *node)
{
	int	pipe_fd[2];
	t_treenode *cmd_node;
	
	if (node->cmd_type == PIPE)
	{
		if (pipe(pipe_fd) == -1)
		{
			cmd_node = node->left;
			while (cmd_node->cmd_type == REDIR_IN || cmd_node->cmd_type == REDIR_OUT
				|| cmd_node->cmd_type == REDIR_APPEND || cmd_node->cmd_type == PIPE
				|| cmd_node->cmd_type == HEREDOC)
			{
				if (cmd_node->right)
					cmd_node = cmd_node->right;
				else if (cmd_node->left)
					cmd_node = cmd_node->left;
				else
					break;
			}
			cmd_node->err_val = errno;
			cmd_node->err = "pipe error";
		}
		else
		{
		cmd_node = node->left;
		while (cmd_node->cmd_type == REDIR_IN || cmd_node->cmd_type == REDIR_OUT
			|| cmd_node->cmd_type == REDIR_APPEND || cmd_node->cmd_type == PIPE
			|| cmd_node->cmd_type == HEREDOC)
		{
			if (cmd_node->right)
				cmd_node = cmd_node->right;
			else if (cmd_node->left)
				cmd_node = cmd_node->left;
			else
				break;
		}
		cmd_node->out_fd = pipe_fd[1];
		cmd_node = node->right;
		while (cmd_node->cmd_type == REDIR_IN || cmd_node->cmd_type == REDIR_OUT
			|| cmd_node->cmd_type == REDIR_APPEND || cmd_node->cmd_type == PIPE
			|| cmd_node->cmd_type == HEREDOC)
		{
			if (cmd_node->right)
				cmd_node = cmd_node->right;
			else if (cmd_node->left)
				cmd_node = cmd_node->left;
			else
				break;
		}
		cmd_node->in_fd = pipe_fd[0];
		}
	}
	if (is_redir(node->cmd_type))
		setup_redir(node);
	if (node->left)
		setup_fd(node->left);
	if (node->right)
		setup_fd(node->right);
}

void	get_input(char *delimiter, int write_fd)
{
	char	*line;
	char	*tmp;
	
	write(0, "> ", 2);
	line = get_next_line(0);
	while (line != NULL)
	{
		tmp = ft_strtrim(line, "\n");
		if (ft_strncmp(tmp, delimiter, ft_strlen(delimiter)) == 0
			&& ft_strlen(tmp) == ft_strlen(delimiter))
			break ;
		free(tmp);
		write(write_fd, line, ft_strlen(line));
		free(line);
		write(0, "> ", 2);
		line = get_next_line(0);
	}
	free(tmp);
	free(line);
	close(write_fd);
}

void	setup_redir(t_treenode *node)
{
	int			tmp_fd;
	t_treenode *cmd_node;
	int			pipe_fd[2];

	if (node->cmd_type == REDIR_IN || node->cmd_type == HEREDOC)
	{
		if (node->cmd_type == REDIR_IN)
			tmp_fd = open(node->args, O_RDONLY);
		else
		{
			if (pipe(pipe_fd) == -1)
			{
				cmd_node = node->left;
				while (cmd_node->cmd_type == REDIR_IN || cmd_node->cmd_type == REDIR_OUT
					|| cmd_node->cmd_type == REDIR_APPEND || cmd_node->cmd_type == HEREDOC)
				{
					if (cmd_node->right)
						cmd_node = cmd_node->right;
					else if (cmd_node->left)
						cmd_node = cmd_node->left;
					else
						break;
				}
				cmd_node->err_val = errno;
				cmd_node->err = "pipe error";
			}
			get_input(node->args, pipe_fd[1]);
			tmp_fd = pipe_fd[0];
		}
		if (tmp_fd == -1)
		{
			cmd_node = node->left;
			while (cmd_node->cmd_type == REDIR_IN || cmd_node->cmd_type == REDIR_OUT
				|| cmd_node->cmd_type == REDIR_APPEND || cmd_node->cmd_type == HEREDOC)
			{
				if (cmd_node->right)
					cmd_node = cmd_node->right;
				else if (cmd_node->left)
					cmd_node = cmd_node->left;
				else
					break;
			}
			if (!cmd_node->err_val || (cmd_node->err_val && ft_strcmp(cmd_node->err, "pipe error") != 0))
			{
				cmd_node->err_val = errno;
				cmd_node->err = node->args;
			}
		}
		else
		{
			cmd_node = node->left;
			while (cmd_node->cmd_type == REDIR_IN || cmd_node->cmd_type == REDIR_OUT
				|| cmd_node->cmd_type == REDIR_APPEND || cmd_node->cmd_type == HEREDOC)
			{
				if (cmd_node->right)
					cmd_node = cmd_node->right;
				else if (cmd_node->left)
					cmd_node = cmd_node->left;
				else
					break;
			}
			if (cmd_node->in_type == 1)
				close(tmp_fd);
			else
			{
				cmd_node->in_fd = tmp_fd;
				cmd_node->in_type = 1;
			}
		}
	}
	else if (node->cmd_type == REDIR_OUT || node->cmd_type == REDIR_APPEND)
	{
		if (node->cmd_type == REDIR_OUT)
			tmp_fd = open(node->args, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			tmp_fd = open(node->args, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (tmp_fd == -1)
		{
			cmd_node = node->left;
			while (cmd_node->cmd_type == REDIR_OUT || cmd_node->cmd_type == REDIR_APPEND
				|| cmd_node->cmd_type == HEREDOC || cmd_node->cmd_type == REDIR_IN)
			{
				if (cmd_node->right)
					cmd_node = cmd_node->right;
				else if (cmd_node->left)
					cmd_node = cmd_node->left;
				else
					break;
			}
			if (!cmd_node->err_val || (cmd_node->err_val && ft_strcmp(cmd_node->err, "pipe error") != 0))
			{
				cmd_node->err_val = errno;
				cmd_node->err = node->args;
			}
		}
		else
		{
			cmd_node = node->left;
			while (cmd_node->cmd_type == REDIR_OUT || cmd_node->cmd_type == REDIR_APPEND 
				|| cmd_node->cmd_type == HEREDOC || cmd_node->cmd_type == REDIR_IN)
			{
				if (cmd_node->right)
					cmd_node = cmd_node->right;
				else if (cmd_node->left)
					cmd_node = cmd_node->left;
				else
					break;
			}
			if (cmd_node->out_type == 1)
				close(tmp_fd);
			else
			{
				cmd_node->out_fd = tmp_fd;
				cmd_node->out_type = 1;
			}
		}
	}
}

static int	execute_execve(t_treenode *ast, char **env_vars, t_pid_list **pid_list)
{
	pid_t	pid;
	char	**arg_arr;
	char	*cmd_node;

	cmd_node = ft_strjoin(ast->cmd, " ");
	if (!cmd_node)
		return 1;
	if (ast->args)
		arg_arr = ft_split(ft_strjoin(cmd_node, ast->args), ' ');
	else
		arg_arr = ft_split(cmd_node, ' ');
	if (!arg_arr)
		return 1;
	if (ast->err_val != 0)
	{
		printf("Error: %s: %s\n", ast->err, strerror(ast->err_val));
		return 1;
	}
	pid = fork();
	if (pid == 0)
	{
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
		execve(find_path(ast->cmd, env_vars), arg_arr, env_vars);
		ft_free_2d_arr((void **)arg_arr);
	}
	else
	{
		t_pid_list	*new;
		new = (t_pid_list *)malloc(sizeof(t_pid_list));
		if (!new)
			return 1;
		new->pid = pid;
		new->next = NULL;
		if (*pid_list == NULL)
			*pid_list = new;
		else
		{
			t_pid_list	*tmp;
			tmp = *pid_list;
			while (tmp->next != NULL)
				tmp = tmp->next;
			tmp->next = new;
		}
		// pid_list = NULL;
		// int status;
		// waitpid(pid, &status, 0);
		if (ast->in_fd != 0)
			close(ast->in_fd);
		if (ast->out_fd != 1)
			close(ast->out_fd);
	}
	ft_free_2d_arr((void **)arg_arr);
	return 0;
}

static int	execute_builtin(t_treenode *ast, t_app_data *app)
{
	int		stdin_fd;
	int		stdout_fd;

	if (ast->err_val != 0)
	{
		printf("Error: %s: %s\n", ast->err, strerror(ast->err_val));
		return 1;
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
	if (ft_strcmp(ast->cmd, "cd") == 0)
		builtin_cd(ast->args);
	else if (ft_strcmp(ast->cmd, "pwd") == 0)
		builtin_pwd(ast->args);
	else if (ft_strcmp(ast->cmd, "echo") == 0)
		builtin_echo(ast->args, STDOUT_FILENO);
	else if (ft_strcmp(ast->cmd, "env") == 0)
		builtin_env(ast->args, app->env_vars);
	else if (ft_strcmp(ast->cmd, "exit") == 0)
		builtin_exit(app, 0);
	// else if (ft_strcmp(ast->cmd, "export") == 0)
	// 	builtin_export(ast->args, app->env_vars);
	// else if (ft_strcmp(ast->cmd, "unset") == 0)
	// 	builtin_unset(ast->args, app->env_vars);
	dup2(stdin_fd, STDIN_FILENO);
	close(stdin_fd);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdout_fd);
	return 0; // just for now
}

int	main(int argc, char **argv, char **envp)
{
	t_app_data	app;
	t_token		*tokens;
	t_treenode	*root;
	t_treenode	*ast;

	(void)argc;
	(void)argv;
	app.env_vars = init_envp(envp);
	app.input = ft_strdup("echo ja | cat < in | echo jo");
	tokens = tokenize(&app);
	root = combine_cmds_args(tokens);
	ast = NULL;
	ast = build_ast(ast, root, 0);
	execute(&app, ast);
	return (0);
}
