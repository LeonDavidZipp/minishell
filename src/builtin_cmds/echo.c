/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/11 16:42:47 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_echo(char *args, int out_fd, t_app_data *app)
{
	char	**args_split;
	bool	n_flag;
	bool	non_flag;
	int		i;
	int		len;

	if (!args)
	{
		ft_fprintf(out_fd, "\n");
		return (0);
	}
	args_split = split(args);
	if (!args_split)
		return (1);
	n_flag = false;
	non_flag = false;
	len = ft_null_terminated_arr_len((void **)args_split);
	i = -1;
	while (args_split[++i])
	{
		args_split[i] = expand_and_remove_in_place(args_split[i],
							app->last_exit_code, app->env_vars, 0);
		if (!non_flag && args_split[i][0] == '-' && args_split[i][1] == 'n'
			&& ft_str_includes_only(&args_split[i][2], "n"))
			n_flag = true;
		else
		{
			non_flag = true;
			ft_fprintf(out_fd, "%s", args_split[i]);
			if (i < len - 1)
				ft_fprintf(out_fd, " ");
		}
	}
	if (!n_flag)
		ft_fprintf(out_fd, "\n");
	ft_free_2d_arr((void **)args_split);
	return (0);
}
