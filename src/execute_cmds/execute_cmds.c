/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 12:54:01 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	execute_cmds(t_treenode *ast, char **envp)
{
	t_tokentype	type;

	if (ast->left)
		execute_cmds(ast->left, envp);
	if (ast->right)
		execute_cmds(ast->right, envp);
	if (ast->cmd)
	{
		type = token_type(ast->cmd);
		if (type == OTHER_CMD)
			execute_cmd(ast->cmd, ast->args, envp);
		else if (type == BUILTIN_CMD)
			execute_builtin(ast->cmd, ast->args, envp);
	}
}

void	execute_cmd(char *cmd, char *args, char **envp)
{
	pid_t	pid;
	int		status;
	char	**arg_arr;

	arg_arr = ft_split(args, ' ');
	if (!arg_arr)
		return ;
	pid = fork();
	if (pid == 0)
	{
		execve(cmd, arg_arr, envp);
		free_2d_arr((void **)arg_arr);
		exit(1);
	}
	else
		waitpid(pid, &status, 0);
	free_2d_arr((void **)arg_arr);
}
