/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:53:27 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/27 16:03:11 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	free_app_data(t_app_data *app);
static void	handle_first_arg_non_number(t_app_data *app, char **arg_arr);
static void	handle_too_many_args(t_app_data *app, char **arg_arr);

void	builtin_exit(t_app_data *app, char *args)
{
	// free memory!!!!!!!!!!!!!!!!
	// free memory!!!!!!!!!!!!!!!!
	// free memory!!!!!!!!!!!!!!!!
	char		**arg_arr;

	if (!args)
	{
		// printf("exit\n");
		free_app_data(app);
		exit(0);
	}
	arg_arr = ft_split(args, ' ');
	if (!arg_arr)
		exit(1) ;
	if (!ft_str_isnumber(arg_arr[0]))
		handle_first_arg_non_number(app, arg_arr);
	else if (ft_null_terminated_arr_len((void **)arg_arr) > 1)
		handle_too_many_args(app, arg_arr);
	else
	{
	free_app_data(app);
	// printf("exit\n");
	exit(ft_atoi(arg_arr[0]));
	}
}

static void	free_app_data(t_app_data *app_data)
{
	ft_free_2d_arr((void **)app_data->env_vars);
	app_data->env_vars = NULL;
	free(app_data->input);
	app_data->input = NULL;
	// free(app_data);
}

static void	handle_first_arg_non_number(t_app_data *app, char **arg_arr)
{
	// printf("exit\n");
	ft_fprintf(2, "%s: exit: %s: numeric argument required\n",
			NAME, arg_arr[0]);
	ft_free_2d_arr((void **)arg_arr);
	free_app_data(app);
	exit(255);
}

static void	handle_too_many_args(t_app_data *app, char **arg_arr)
{
	// printf("exit\n");
	ft_fprintf(2, "%s: exit: too many arguments\n", NAME);
	ft_free_2d_arr((void **)arg_arr);
	free_app_data(app);
	// exit(ft_atoi(arg_arr[0]));
	exit(1);
}

// 
