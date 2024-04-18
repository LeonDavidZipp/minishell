/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:55:20 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/18 11:58:05 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_exit(bool noninteractive)
{
	if (!noninteractive)
		ft_printf("exit\n");
}

void	free_app_data(t_app_data *app_data)
{
	ft_free_2d_arr((void **)app_data->env_vars);
	app_data->env_vars = NULL;
	free(app_data->input);
	app_data->input = NULL;
}

void	handle_first_arg_non_number(t_app_data *app, char **arg_arr)
{
	print_exit(app->noninteractive);
	ft_fprintf(2, "%s: exit: %s: numeric argument required\n",
		NAME, arg_arr[0]);
	ft_free_2d_arr((void **)arg_arr);
	free_app_data(app);
	exit(255);
}

int	handle_too_many_args(t_app_data *app, char **arg_arr)
{
	print_exit(app->noninteractive);
	ft_fprintf(2, "%s: exit: %s: numeric argument required\n",
		NAME, arg_arr[0]);
	ft_free_2d_arr((void **)arg_arr);
	return (1);
}

void	handle_split_fail(t_app_data *app, char **arg_arr)
{
	print_exit(app->noninteractive);
	ft_free_2d_arr((void **)arg_arr);
	free_app_data(app);
	exit(1);
}
