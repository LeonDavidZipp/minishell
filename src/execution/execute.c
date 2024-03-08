/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/08 18:46:00 by cgerling         ###   ########.fr       */
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
	
	if (node->cmd_type == PIPE)
	{
		// if (prev_node && prev_node->out_fd != 1)
		// 	node->left->in_fd = prev_node->out_fd;
		if (pipe(pipe_fd) == -1)
		{
			node->left->err_val = errno;
			node->right->err_val = -1;
		}
		else
		{
			if (node->left->cmd_type == PIPE)
				node->left->right->out_fd = pipe_fd[1];
			else
				node->left->out_fd = pipe_fd[1];
			node->right->in_fd = pipe_fd[0];
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
	int	tmp_fd;

	if (node->cmd_type == REDIR_IN)
	{
		if (prev_node)
		{
			if (prev_node->cmd_type == REDIR_IN && prev_node->in_fd == 0)
			{
				node->left->in_fd = open(node->args, O_RDONLY);
				if (node->left->in_fd == -1)
					node->left->err_val = errno;
			}
			else
			{
				node->left->in_fd = prev_node->in_fd;
				tmp_fd = open(node->args, O_RDONLY);
				if (tmp_fd == -1 || node->err_val > 0)
				{
					if (node->err_val > 0)
					{
						node->left->err_val = node->err_val;
						node->left->err = node->err;
					}
					else
					{
						node->left->err_val = errno;
						node->left->err = node->args;
					}
				}
				close(tmp_fd);
			}
		}
		else
		{
			node->left->in_fd = open(node->args, O_RDONLY);
			if (node->left->in_fd == -1)
			{
				node->left->err_val = errno;
				node->left->err = node->args;
			}
			node->in_fd = node->left->in_fd;
		}
	}
	else if (node->cmd_type == REDIR_OUT || node->cmd_type == REDIR_APPEND)
	{
		if (prev_node)
		{
			if ((prev_node->cmd_type == REDIR_OUT || prev_node->cmd_type == REDIR_APPEND) && prev_node->out_fd == 1)
			{
				if (node->cmd_type == REDIR_OUT)
					node->left->out_fd = open(node->args, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				else
					node->left->out_fd = open(node->args, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (node->left->out_fd == -1)
				{
					node->left->err_val = errno;
					node->left->err = node->args;
				}
			}
			else
			{
				node->left->out_fd = prev_node->out_fd;
				if (node->cmd_type == REDIR_OUT)
					tmp_fd = open(node->args, O_WRONLY | O_CREAT | O_TRUNC, 0644);
					tmp_fd = open(node->args, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (tmp_fd == -1 || node->err_val > 0) 
				{
					if (node->err_val > 0)
					{
						node->left->err_val = node->err_val;
						node->left->err = node->err;
					}
					else
					{
						node->left->err_val = errno;
						node->left->err = node->args;
					}
				}
				close(tmp_fd);
			}
		}
		else
		{
			if (node->cmd_type == REDIR_OUT)
				node->left->out_fd = open(node->args, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				node->left->out_fd = open(node->args, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (node->left->out_fd == -1)
			{
				node->left->err_val = errno;
				node->left->err = node->args;
			}
			node->out_fd = node->left->out_fd;
		}
	}
}

void	execute_execve(t_treenode *ast, char **env_vars)
{
	pid_t	pid;
	int		status;
	char	**arg_arr;
	char	*tmp;

	tmp = ft_strjoin(ast->cmd, " ");
	if (!tmp)
		return ;
	if (ast->args)
		arg_arr = ft_split(ft_strjoin(tmp, ast->args), ' ');
	else
		arg_arr = ft_split(tmp, ' ');
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
	app.input = ft_strdup("echo hello | ls -l | wc -l > out"); // when updated version check if (< in cat -e) segfaults
	tokens = tokenize(&app);
	root = combine_cmds_args(tokens);
	ast = NULL;
	ast = build_ast(ast, root, 0);
	traverse_tree(ast, NULL);
	debug_printtree(ast, 0);
	exec_cmds(ast, &app);
	return (0);
}
