/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/15 16:43:21 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_echo(char *args, int out_fd, t_app_data **app)
{
	char	**args_split;
	bool	n_flag;
	bool	non_flag;
	int		i;
	int		len;

	// printf("args: |%s|\n", args);
	if (!args)
		return (ft_fprintf(out_fd, "\n"), 0);
	int	flags[2];
	flags[0] = 0;
	flags[1] = 0;
	char *tmp = expand(args, (*app)->last_exit_code, (*app)->env_vars, flags);
	args_split = split(tmp);
	// args_split = split(args);
	if (!args_split)
		return (1);
	// for (int i = 0; args_split[i]; i++)
	// {
	// 	printf("args_split[%d]: |%s|\n", i, args_split[i]);
	// }
	n_flag = false;
	non_flag = false;
	len = ft_null_terminated_arr_len((void **)args_split);
	i = -1;
	while (args_split[++i])
	{
		args_split[i] = expand_and_remove_in_place(args_split[i],
							(*app)->last_exit_code, (*app)->env_vars, 0);
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
	return (ft_free_2d_arr((void **)args_split), 0);
}
