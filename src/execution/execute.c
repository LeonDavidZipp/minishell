/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/07 16:56:51 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	execute_builtin(char *cmd, char *args, char **env_vars);
void		execute_execve(char *cmd, char *args, char **env_vars);
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

// void	execute(t_treenode *ast, t_app_data *app)
// {
// 	traverse_tree(ast, NULL); // this function is for setting the file descriptors for the commands if there are pipes or redirections
// 	exec_cmds(ast, app);
// }

// need to implement heredoc separately from the rest of the redirections

void	exec_cmds(t_treenode *ast, t_app_data *app)
{
	if (ast->left)
		exec_cmds(ast->left, app);
	if (ast->right)
		exec_cmds(ast->right, app);
	if (ast->cmd_type == CMD)
	{
		if (is_builtin(ast->cmd))
			execute_builtin(ast->cmd, ast->args, app->env_vars);
		else
			execute_execve(ast->cmd, ast->args, app->env_vars);
	}
}

void	traverse_tree(t_treenode *node, t_treenode *prev_node)
{
	int	pipe_fd[2];
	pid_t	pid;
	
	if (node->cmd_type == PIPE)
	{
		if (prev_node)
		{
			if (prev_node->out_fd != 1)
			{
				node->left->in_fd = prev_node->out_fd;
			}
		}
		if (pipe(pipe_fd) == -1)
			error_exit("Error: pipe failed\n", 1);
		node->left->out_fd = pipe_fd[1];
		node->right->in_fd = pipe_fd[0];
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
	if (node->cmd_type == REDIR_IN)
	{
		if (prev_node)
		{
			if (prev_node->in_fd == 0)
				node->in_fd = open(node->args, O_RDONLY);
		}
	}
}

void	execute_execve(char *cmd, char *args, char **env_vars)
{
	pid_t	pid;
	int		status;
	char	**arg_arr;
	char	*tmp;

	tmp = ft_strjoin(cmd, " ");
	if (!tmp)
		return ;
	if (args)
		arg_arr = ft_split(ft_strjoin(tmp, args), ' ');
	else
		arg_arr = ft_split(tmp, ' ');
	if (!arg_arr)
		return ;
	pid = fork();
	if (pid == 0)
	{
		execve(find_path(cmd, env_vars), arg_arr, env_vars);
		ft_free_2d_arr((void **)arg_arr);
		ft_free_2d_arr((void **)env_vars);
		exit(1);
	}
	else
		waitpid(pid, &status, 0);
	ft_free_2d_arr((void **)arg_arr);
}

static void	execute_builtin(char *cmd, char *args, char **env_vars)
{
	if (ft_strcmp(cmd, "cd") == 0)
		builtin_cd(args);
	else if (ft_strcmp(cmd, "pwd") == 0)
		builtin_pwd(args);
	else if (ft_strcmp(cmd, "echo") == 0)
		builtin_echo(args);
	else if (ft_strcmp(cmd, "env") == 0)
		builtin_env(args, env_vars);
	else if (ft_strcmp(cmd, "exit") == 0)
		builtin_exit(0);
	// else if (ft_strcmp(cmd, "export") == 0)
	// 	builtin_export(args, env_vars);
	// else if (ft_strcmp(cmd, "unset") == 0)
	// 	builtin_unset(args, env_vars);
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
	app.input = ft_strdup("echo hi > out > in");
	tokens = tokenize(&app);
	root = combine_cmds_args(tokens);
	ast = NULL;
	ast = build_ast(ast, root, 0);
	debug_printtree(ast, 0);
	// execute(ast, &app);
	return (0);
}
