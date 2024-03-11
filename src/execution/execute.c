/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/11 18:29:11 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	execute_builtin(t_treenode *ast, t_app_data *app);
static int	execute_execve(t_treenode *ast, char **env_vars, t_pid_list *pid_list);
void		exec_cmds(t_treenode *ast, t_app_data *app, t_pid_list *pid_list);
void		setup_redir(t_treenode *node);
void		setup_fd(t_treenode *node);

// init_environ file funtion get_path i think is not needed anymore
char	*find_path(char *command, char **envp);

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
		|| type == REDIR_APPEND)
		return (1);
	return (0);
}

void	execute(t_app_data *app, t_treenode *ast)
{
	t_pid_list	*pid_list;
	// int			status;

	pid_list = NULL;
	setup_fd(ast);
	// debug_printtree(ast, 0);
	app->last_exit_code = 0; // just for now
	exec_cmds(ast, app, pid_list);
	// while (pid_list)
	// {
	// 	waitpid(pid_list->pid, &status, 0);
	// 	if (WIFEXITED(status))
	// 		app->last_exit_code = WEXITSTATUS(status);
	// 	else if (WIFSIGNALED(status))
	// 		app->last_exit_code = 128 + WTERMSIG(status);
	// 	pid_list = pid_list->next;
	// }
}

// void	exec_cmds(t_treenode *ast, t_app_data *app, t_pid_list *pid_list)
// {
// 	if (ast == NULL)
// 		return ;
// 	if (ast->cmd_type == CMD)
// 	{
// 		if (is_builtin(ast->cmd))
// 			execute_builtin(ast, app);
// 		else
// 			execute_execve(ast, app->env_vars, pid_list);
// 	}
// 	if (ast->left)
// 		exec_cmds(ast->left, app, pid_list);
// 	if (ast->right)
// 		exec_cmds(ast->right, app, pid_list);
// }

// void exec_cmds(t_treenode *ast, t_app_data *app, t_pid_list *pid_list)
// {
// 	if (ast == NULL)
// 		return;
// 	if (ast->left)
// 		exec_cmds(ast->left, app, pid_list);
// 	if (ast->cmd_type == CMD)
// 	{
// 		if (is_builtin(ast->cmd))
// 			execute_builtin(ast, app);
// 		else
// 			execute_execve(ast, app->env_vars, pid_list);
// 	}
// 	else if (ast->cmd_type == AND && app->last_exit_code == 0)
// 		exec_cmds(ast->right, app, pid_list);
// 	else if (ast->cmd_type == OR && app->last_exit_code != 0)
// 		exec_cmds(ast->right, app, pid_list);
// 	if (ast->cmd_type != AND && ast->cmd_type != OR)
// 	{
// 		if (ast->right)
// 			exec_cmds(ast->right, app, pid_list);
// 	}
// }

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

void	setup_redir(t_treenode *node)
{
	int			tmp_fd;
	t_treenode *cmd_node;

	if (node->cmd_type == REDIR_IN || node->cmd_type == HEREDOC)
	{
		if (node->cmd_type == REDIR_IN)
			tmp_fd = open(node->args, O_RDONLY);
		else
			tmp_fd = 0;
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

static int	execute_execve(t_treenode *ast, char **env_vars, t_pid_list *pid_list)
{
	pid_t	pid;
	int		status;
	char	**arg_arr;
	char	*cmd_node;

	pid_list = NULL; // just for now
	cmd_node = ft_strjoin(ast->cmd, " ");
	if (!cmd_node)
		return 1;
	if (ast->args)
		arg_arr = ft_split(ft_strjoin(cmd_node, ast->args), ' ');
	else
		arg_arr = ft_split(cmd_node, ' ');
	if (!arg_arr)
		return 1;
	pid = fork();
	if (pid == 0)
	{
		if (ast->err_val > 0)
		{
			printf("Error: %s: %s\n", ast->err, strerror(ast->err_val));
			return 1;
		}
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
		waitpid(pid, &status, 0);
		if (ast->in_fd != 0)
			close(ast->in_fd);
		if (ast->out_fd != 1)
			close(ast->out_fd);
	}
	ft_free_2d_arr((void **)arg_arr);
	return 0;
}

static void	execute_builtin(t_treenode *ast, t_app_data *app)
{
	int		stdin_fd;
	int		stdout_fd;

	if (ast->err_val != 0)
	{
		printf("Error: %s: %s\n", ast->err, strerror(ast->err_val));
		return ;
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
	app.input = ft_strdup("echo 1 || (echo 2 && echo 3)");
	tokens = tokenize(&app);
	root = combine_cmds_args(tokens);
	ast = NULL;
	ast = build_ast(ast, root, 0);
	execute(&app, ast);
	return (0);
}
