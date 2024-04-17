/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/17 18:51:13 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_for_errors(t_treenode *ast, int last_exit_code);

int	execute(t_app_data *app, t_treenode *ast)
{
	t_pid_list	*pid_list;
	int			ret;

	ret = 0;
	pid_list = NULL;
	ret = 0;
	setup_fd(ast, app, &ret);
	if (ret == 2 || g_exit_signal == 2)
	{
		app->last_exit_code = 1;
		return (1);
	}
	exec_cmds(ast, app, &pid_list);
	wait_and_free(app, &pid_list);
	return (0);
}

void	exec_cmds(t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
{
	if (ast == NULL)
		return ;
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
