/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/16 11:44:52 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	print_args(t_app_data **app, char ***args_split, int out_fd);

int	builtin_echo(char *args, int out_fd, t_app_data **app)
{
	char	**args_split;
	int		flags[2];
	char	*tmp;

	if (!args)
		return (ft_fprintf(out_fd, "\n"), 0);
	flags[0] = 0;
	flags[1] = 0;
	tmp = expand(args, (*app)->last_exit_code, (*app)->env_vars, flags);
	args_split = split(tmp);
	if (!args_split)
		return (1);
	if (!print_args(app, &args_split, out_fd))
		ft_fprintf(out_fd, "\n");
	return (ft_free_2d_arr((void **)args_split), 0);
}

static bool	print_args(t_app_data **app, char ***args_split, int out_fd)
{
	int			i;
	int			len;
	bool		n_flag;
	bool		no_n_flag;

	n_flag = false;
	no_n_flag = false;
	i = -1;
	len = ft_null_terminated_arr_len((void **)(*args_split));
	while ((*args_split)[++i])
	{
		(*args_split)[i] = expand_and_remove_in_place((*args_split)[i],
				(*app)->last_exit_code, (*app)->env_vars, 0);
		if ((*args_split)[i][0] == '-' && (*args_split)[i][1] == 'n'
			&& ft_str_includes_only(&(*args_split)[i][2], "n") && !no_n_flag)
			n_flag = true;
		else
		{
			no_n_flag = true;
			ft_fprintf(out_fd, "%s", (*args_split)[i]);
			if (i < len - 1)
				ft_fprintf(out_fd, " ");
		}
	}
	return (n_flag);
}
