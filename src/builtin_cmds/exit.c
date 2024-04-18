/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:53:27 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/18 11:56:33 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_exit(t_app_data *app, char *args)
{
	char		**arg_arr;
	int			exit_code;

	if (!args)
	{
		print_exit(app->noninteractive);
		free_app_data(app);
		exit(0);
	}
	arg_arr = ft_split(args, ' ');
	if (!arg_arr)
		handle_split_fail(app, arg_arr);
	else if (!ft_str_isnumber(arg_arr[0]))
		handle_first_arg_non_number(app, arg_arr);
	else if (ft_null_terminated_arr_len((void **)arg_arr) > 1)
		return (handle_too_many_args(app, arg_arr));
	else
	{
		print_exit(app->noninteractive);
		exit_code = ft_atoi(arg_arr[0]) % 256;
		ft_free_2d_arr((void **)arg_arr);
		free_app_data(app);
		exit(exit_code);
	}
	return (0);
}
