/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/09 17:06:27 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	execute_builtin(t_treenode *ast, char **env_vars);
void		execute_execve(t_treenode *ast, char **env_vars);
void		exec_cmds(t_treenode *ast, t_app_data *app);
void		setup_redir(t_treenode *node, t_treenode *prev_node);
void		traverse_tree(t_treenode *node, t_treenode *prev_node);

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

// need to implement heredoc separately from the rest of the redirections

// need to put in another check, so that if there was a pipe before, meaning the out_fd is not default i stil need to open the file and update the fd variables
// (muss mir was schlaues überlegen) P.S. not only here but for all redirections and maype pipes?
// muss die previous typen abchecken und je nachdem ...

void	exec_cmds(t_treenode *ast, t_app_data *app)
{
	if (ast->left)
		exec_cmds(ast->left, app);
	if (ast->right)
		exec_cmds(ast->right, app);
	if (ast->cmd_type == CMD)
	{
		if (is_builtin(ast->cmd))
			execute_builtin(ast, app->env_vars);
		else
			execute_execve(ast, app->env_vars);
	}
}

void	traverse_tree(t_treenode *node, t_treenode *prev_node)
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
		// printf("this is the cmd_node: %s and the new out_fd is %d\n", cmd_node->cmd, pipe_fd[1]);
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
		// printf("this is the node->right: %s and the new in_fd is %d\n", node->right->cmd, pipe_fd[0]);
		cmd_node->in_fd = pipe_fd[0];
		}
	}
	if (is_redir(node->cmd_type))
		setup_redir(node, prev_node);
	if (node->left)
		traverse_tree(node->left, node);
	if (node->right)
		traverse_tree(node->right, node);
}

void	setup_redir(t_treenode *node, t_treenode *prev_node)
{
	int			tmp_fd;
	t_treenode *cmd_node;

	if (node->cmd_type == REDIR_IN)
	{
		if (prev_node)
		{
			tmp_fd = open(node->args, O_RDONLY);
			if (tmp_fd == -1)
			{
				cmd_node = node->left;
				while (cmd_node->cmd_type == REDIR_IN)
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
					printf("setting err_val to errno at node %s\n", cmd_node->cmd);
					cmd_node->err_val = errno;
					cmd_node->err = node->args;
				}
			}
			else
			{
				// printf("opened tmp_fd: %d\n", tmp_fd);
				if (prev_node->cmd_type != REDIR_IN)
				{
					cmd_node = node->left;
					while (cmd_node->cmd_type == REDIR_IN)
					{
						if (cmd_node->right)
							cmd_node = cmd_node->right;
						else if (cmd_node->left)
							cmd_node = cmd_node->left;
						else
							break;
					}
					// printf("this would be the cmd_node: %s\n", cmd_node->cmd);
					// printf("node_fd: %d will now change to %d\n", node->in_fd, tmp_fd);
					cmd_node->in_fd = tmp_fd;
				}
				else
				{
					close(tmp_fd);
					// printf("closed tmp_fd %d\n", tmp_fd);
				}
			}
		}
		else
		{
			tmp_fd = open(node->args, O_RDONLY);
			if (tmp_fd == -1)
			{
				cmd_node = node->left;
				while (cmd_node->cmd_type == REDIR_IN)
				{
					if (cmd_node->right)
						cmd_node = cmd_node->right;
					else if (cmd_node->left)
						cmd_node = cmd_node->left;
					else
						break;
				}
				cmd_node->err_val = errno;
				cmd_node->err = node->args;
			}
			else
			{
				cmd_node = node->left;
				while (cmd_node->cmd_type == REDIR_IN)
				{
					if (cmd_node->right)
						cmd_node = cmd_node->right;
					else if (cmd_node->left)
						cmd_node = cmd_node->left;
					else
						break;
				}
				cmd_node->in_fd = tmp_fd;
			}
		}
	}
	else if (node->cmd_type == REDIR_OUT || node->cmd_type == REDIR_APPEND)
	{
		if (prev_node)
		{
			if (node->cmd_type == REDIR_OUT)
				tmp_fd = open(node->args, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				tmp_fd = open(node->args, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (tmp_fd == -1)
			{
				cmd_node = node->left;
				while (cmd_node->cmd_type == REDIR_OUT || cmd_node->cmd_type == REDIR_APPEND)
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
				if (prev_node->cmd_type != REDIR_OUT && prev_node->cmd_type != REDIR_APPEND)
				{
					cmd_node = node->left;
					while (cmd_node->cmd_type == REDIR_OUT || cmd_node->cmd_type == REDIR_APPEND)
					{
						if (cmd_node->right)
							cmd_node = cmd_node->right;
						else if (cmd_node->left)
							cmd_node = cmd_node->left;
						else
							break;
					}
					cmd_node->out_fd = tmp_fd;
				}
				else
				{
					close(tmp_fd);
				}
			}
		}
		else
		{
			if (node->cmd_type == REDIR_OUT)
				tmp_fd = open(node->args, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				tmp_fd = open(node->args, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (tmp_fd == -1)
			{
				cmd_node = node->left;
				while (cmd_node->cmd_type == REDIR_OUT || cmd_node->cmd_type == REDIR_APPEND)
				{
					if (cmd_node->right)
						cmd_node = cmd_node->right;
					else if (cmd_node->left)
						cmd_node = cmd_node->left;
					else
						break;
				}
				cmd_node->err_val = errno;
				cmd_node->err = node->args;
			}
			else
			{
				cmd_node = node->left;
				while (cmd_node->cmd_type == REDIR_OUT || cmd_node->cmd_type == REDIR_APPEND)
				{
					if (cmd_node->right)
						cmd_node = cmd_node->right;
					else if (cmd_node->left)
						cmd_node = cmd_node->left;
					else
						break;
				}
				cmd_node->out_fd = tmp_fd;
			}
		}
	}
}

void	execute_execve(t_treenode *ast, char **env_vars)
{
	pid_t	pid;
	int		status;
	char	**arg_arr;
	char	*cmd_node;

	cmd_node = ft_strjoin(ast->cmd, " ");
	if (!cmd_node)
		return ;
	if (ast->args)
		arg_arr = ft_split(ft_strjoin(cmd_node, ast->args), ' ');
	else
		arg_arr = ft_split(cmd_node, ' ');
	if (!arg_arr)
		return ;
	pid = fork();
	if (pid == 0)
	{
		if (ast->err_val > 0)
		{
			printf("Error: %s: %s\n", ast->err, strerror(ast->err_val));
			return ;
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
		ft_free_2d_arr((void **)env_vars);
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
}

static void	execute_builtin(t_treenode *ast, char **env_vars)
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
		builtin_env(ast->args, env_vars);
	else if (ft_strcmp(ast->cmd, "exit") == 0)
		builtin_exit(0);
	// else if (ft_strcmp(ast->cmd, "export") == 0)
	// 	builtin_export(ast->args, env_vars);
	// else if (ft_strcmp(ast->cmd, "unset") == 0)
	// 	builtin_unset(ast->args, env_vars);
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
	app.input = ft_strdup("ls -l | wc -l > out > out2");
	tokens = tokenize(&app);
	root = combine_cmds_args(tokens);
	ast = NULL;
	ast = build_ast(ast, root, 0);
	traverse_tree(ast, NULL);
	//debug_printtree(ast, 0);
	exec_cmds(ast, &app);
	return (0);
}
