/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_execve_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 17:44:42 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/17 17:47:10 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
