/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 15:08:59 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	execute_execv(char *cmd, char *args, t_env_var *env_vars);

void	execute_cmds(t_treenode *ast, t_env_var *env_vars)
{
	t_tokentype	type;
	char		*path;

	if (ast->left)
		execute_cmds(ast->left, env_vars);
	if (ast->right)
		execute_cmds(ast->right, env_vars);
	if (ast->cmd)
	{
		path = get_path(env_vars);
		type = token_type(ast->cmd, path);
		if (type == OTHER_CMD)
			execute_execv(ast->cmd, ast->args, env_vars);
		else if (type == BUILTIN_CMD)
			execute_builtin(ast->cmd, ast->args, env_vars);
	}
}

static void	execute_execv(char *cmd, char *args, t_env_var *env_vars)
{
	pid_t	pid;
	int		status;
	char	**arg_arr;
	char	**envp;

	arg_arr = ft_split(args, ' ');
	if (!arg_arr)
		return ;
	pid = fork();
	if (pid == 0)
	{
		envp = env_vars_to_char_arr(env_vars);
		if (!envp)
			return ;
		execve(cmd, arg_arr, envp);
		free_2d_arr((void **)arg_arr);
		free_2d_arr((void **)envp);
		exit(1);
	}
	else
		waitpid(pid, &status, 0);
	free_2d_arr((void **)arg_arr);
}

static void	execute_builtin(char *cmd, char *args, t_env_var *env_vars)
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
	else if (ft_strcmp(cmd, "export") == 0)
		builtin_export(args, &env_vars);
	else if (ft_strcmp(cmd, "unset") == 0)
		builtin_unset(args, &env_vars);
}