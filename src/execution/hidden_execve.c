/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hidden_execve.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:02:08 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/17 11:08:09 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	reassign_arg_arr(char ***arg_arr, t_app_data **app)
{
	int			i;
	char		*tmp;

	i = 0;
	while ((*arg_arr)[i])
	{
		tmp = expand_and_remove((*arg_arr)[i], (*app)->last_exit_code,
				(*app)->env_vars, 0);
		if (!tmp)
			return (1);
		free((*arg_arr)[i]);
		(*arg_arr)[i] = tmp;
		i++;
	}
	return (0);
}

static int	handle_execve(pid_t pid, char ***arg_arr, t_app_data **app,
				t_pid_list **pid_list)
{
	struct stat		path_stat;
	bool			flag;

	if (pid == 0)
	{
		stat((*arg_arr)[0], &path_stat);
		if (S_ISDIR(path_stat.st_mode))
		{
			ft_fprintf(2, "%s: %s: is a directory\n", NAME, (*arg_arr)[0]);
			exit(126);
		}
		flag = false;
		if (access((*arg_arr)[0], X_OK) == 0)
			execve((*arg_arr)[0], *arg_arr, (*app)->env_vars);
		else
			execve(find_path((*arg_arr)[0], (*app)->env_vars, &flag),
				(*arg_arr), (*app)->env_vars);
		exit(127);
	}
	else
	{
		if (add_to_pid_list(pid, pid_list))
			return (1);
	}
	return (0);
}

int	hidden_execve(char *hidden_command, t_app_data *app, t_pid_list **pid_list)
{
	pid_t	pid;
	char	*tmp;
	char	**arg_arr;
	int		flags[2];

	flags[0] = 0;
	flags[1] = 1;
	tmp = expand(hidden_command, app->last_exit_code,
			app->env_vars, flags);
	if (!tmp)
		return (1);
	arg_arr = split(tmp);
	free(tmp);
	if (reassign_arg_arr(&arg_arr, &app) == 1)
		return (ft_free_2d_arr((void **)arg_arr), 1);
	pid = fork();
	if (pid == -1)
		return (ft_fprintf(2, "%s: fork error: %s\n", NAME,
				strerror(errno)), 1);
	if (handle_execve(pid, &arg_arr, &app, pid_list) == 1)
		return (ft_free_2d_arr((void **)arg_arr), 1);
	ft_free_2d_arr((void **)arg_arr);
	return (0);
}
