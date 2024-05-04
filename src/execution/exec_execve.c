/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execve.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:15:11 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/17 18:48:43 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int		execute_command(t_treenode *ast, t_execve *var, t_app_data *app,
					t_pid_list **pid_list);
static int		handle_hidden_cmd(t_execve *var, t_app_data *app,
					t_pid_list **pid_list);
static void		child_process(t_treenode *ast, t_execve *var, t_app_data *app);
static int		parent_process(t_treenode *ast, t_execve *var,
					t_pid_list **pid_list);
t_execve		init_var(t_treenode *ast);

int	execute_execve(t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
{
	t_execve	var;

	if (ast->err_val != 0)
		return (handle_error(ast), 1);
	var = init_var(ast);
	if (!var.cmd_node)
		return (1);
	if (ast->args)
	{
		var.arg_arr = set_args(&var, ast, app);
		if (!var.arg_arr)
			return (free(var.cmd_node), 1);
	}
	else
		var.arg_arr = split(var.cmd_node);
	if (!var.arg_arr)
		return (free(var.cmd_node), 1);
	return (execute_command(ast, &var, app, pid_list));
}

static int	execute_command(t_treenode *ast, t_execve *var, t_app_data *app,
		t_pid_list **pid_list)
{
	if (exp_rem_arg_arr(var, app))
		return (free(var->cmd_node), ft_free_2d_arr((void **)var->arg_arr), 1);
	if (is_hidden_command(var->arg_arr[0], app->env_vars) && ast->cmd[0] == '$')
		return (handle_hidden_cmd(var, app, pid_list));
	var->pid = fork();
	if (var->pid == -1)
	{
		ft_fprintf(2, "%s: fork error: %s\n", NAME, strerror(errno));
		return (free(var->cmd_node), free((void **)var->arg_arr), 1);
	}
	if (var->pid == 0)
		child_process(ast, var, app);
	else
	{
		if (parent_process(ast, var, pid_list))
			return (1);
	}
	return (free(var->cmd_node), ft_free_2d_arr((void **)var->arg_arr), 0);
}

int	handle_hidden_cmd(t_execve *var, t_app_data *app, t_pid_list **pid_list)
{
	int	ret;

	if (is_hidden_command(var->arg_arr[0], app->env_vars) == 2
		&& !var->arg_arr[1])
		return (free(var->cmd_node), ft_free_2d_arr((void **)var->arg_arr), 0);
	else if (is_hidden_command(var->arg_arr[0], app->env_vars) == 2
		&& var->arg_arr[1])
	{
		ret = exec_hidden_command(var->arg_arr[1], var->arg_arr, app, pid_list);
		free(var->cmd_node);
		return (ft_free_2d_arr((void **)var->arg_arr), ret);
	}
	ret = exec_hidden_command(var->arg_arr[0], var->arg_arr, app, pid_list);
	return (free(var->cmd_node), ft_free_2d_arr((void **)var->arg_arr), ret);
}

static void	child_process(t_treenode *ast, t_execve *var, t_app_data *app)
{
	if (handle_fds_dup2(ast))
		exit(1);
	close_fds_loop();
	handle_stat(var);
	if (access(var->arg_arr[0], X_OK) == 0)
		execve(var->arg_arr[0], var->arg_arr, app->env_vars);
	else
	{
		var->path = find_path(var->arg_arr[0], app->env_vars, &var->flag);
		if (var->path)
			execve(var->path, var->arg_arr, app->env_vars);
	}
	if (var->flag)
		exit(126);
	exit(127);
}

static int	parent_process(t_treenode *ast, t_execve *var, t_pid_list **pid_list)
{
	if (add_to_pid_list(var->pid, pid_list))
		return (free(var->cmd_node), ft_free_2d_arr((void **)var->arg_arr), 1);
	if (ast->in_fd != 0)
		close(ast->in_fd);
	if (ast->out_fd != 1)
		close(ast->out_fd);
	return (0);
}
