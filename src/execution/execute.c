/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/17 16:54:15 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// memory leaks!
// norminette!!!!!!!

// ctrl c sets exit code to 1 used on an empty line
// heredoc ctrl d cant't exit on a line where something is written

char	**set_args(t_execve *var, t_treenode *ast, t_app_data *app)
{
	char	**arg_arr;

	var->tmp = ft_strjoin(var->cmd_node, ast->args);
	if (!var->tmp)
		return (NULL);
	var->temp = expand(var->tmp, app->last_exit_code,
			app->env_vars, var->flags);
	if (!var->temp)
	{
		free(var->tmp);
		return (NULL);
	}
	free(var->tmp);
	var->tmp = expand(var->temp, app->last_exit_code,
			app->env_vars, var->flags);
	if (!var->tmp)
	{
		free(var->temp);
		return (NULL);
	}
	arg_arr = split(var->tmp);
	free(var->tmp);
	free(var->temp);
	return (arg_arr);
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

void	handle_stat(t_execve *var)
{
	if (stat(var->arg_arr[0], &var->path_stat) == 0)
	{
		if (S_ISDIR(var->path_stat.st_mode))
		{
			if (ft_strcmp(var->arg_arr[0], ".") == 0)
			{
				ft_fprintf(2, "%s%s%s", NAME, DOT_MSG, DOT_MSG2);
				exit(127);
			}
			if (ft_strcmp(var->arg_arr[0], "..") == 0)
			{
				ft_fprintf(2, "%s: ..: command not found\n", NAME);
				exit(127);
			}
			ft_fprintf(2, "%s: %s: is a directory\n", NAME, var->arg_arr[0]);
			exit(126);
		}
	}
}

t_execve	init_var(t_treenode *ast)
{
	t_execve	var;

	var.flag = false;
	var.flags[0] = 0;
	var.flags[1] = 0;
	var.cmd_node = ft_strjoin(ast->cmd, " ");
	var.arg_arr = NULL;
	var.i = 0;
	return (var);
}

int	exp_rem_arg_arr(t_execve *var, t_app_data *app)
{
	while (var->arg_arr[var->i])
	{
		var->temp = expand_and_remove(var->arg_arr[var->i],
				app->last_exit_code, app->env_vars, 0);
		if (!var->temp)
			return (1);
		free(var->arg_arr[var->i]);
		var->arg_arr[var->i] = var->temp;
		var->i++;
	}
	return (0);
}

void	child_process(t_treenode *ast, t_execve *var, t_app_data *app)
{
	if (handle_fds_dup2(ast))
			exit(1);
		close_fds_loop();
		handle_stat(var);
		if (access(var->arg_arr[0], X_OK) == 0)
			execve(var->arg_arr[0], var->arg_arr, app->env_vars);
		else
			execve(find_path(var->arg_arr[0], app->env_vars, &var->flag), var->arg_arr, app->env_vars);
		if (var->flag)
			exit(126);
		exit(127);
}

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
	if (exp_rem_arg_arr(&var, app))
		return (free(var.cmd_node), ft_free_2d_arr((void **)var.arg_arr), 1);
	if (is_hidden_command(var.arg_arr[0], app->env_vars) && ast->cmd[0] == '$')
		return (handle_hidden_cmd(&var, app, pid_list));
	var.pid = fork();
	if (var.pid == -1)
	{
		ft_fprintf(2, "%s: fork error: %s\n", NAME, strerror(errno));
		return (free(var.cmd_node), free((void **)var.arg_arr), 1);
	}
	if (var.pid == 0)
		child_process(ast, &var, app);
	else
	{
		if (add_to_pid_list(var.pid, pid_list))
			return (free(var.cmd_node), ft_free_2d_arr((void **)var.arg_arr), 1);
		if (ast->in_fd != 0)
			close(ast->in_fd);
		if (ast->out_fd != 1)
			close(ast->out_fd);
	}
	free(var.cmd_node);
	ft_free_2d_arr((void **)var.arg_arr);
	return (0);
}

void	close_fds_loop(void) // also using this in builtin
{
	int	fd;

	fd = 3;
	while (fd < FD_SETSIZE)
	{
		if (fd != STDIN_FILENO && fd != STDOUT_FILENO)
			close(fd);
		fd++;
	}
}
