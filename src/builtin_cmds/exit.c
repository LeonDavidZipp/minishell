/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:53:27 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/13 13:18:58 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	free_app_data(t_app_data *app);

void	builtin_exit(t_app_data *app, int exit_code)
{
	// free memory!!!!!!!!!!!!!!!!
	// free memory!!!!!!!!!!!!!!!!
	// free memory!!!!!!!!!!!!!!!!
	free_app_data(app);
	printf("exit\n");
	exit(exit_code);
}

static void	free_app_data(t_app_data *app_data)
{
	ft_free_2d_arr((void **)app_data->env_vars);
	app_data->env_vars = NULL;
	free(app_data->input);
	app_data->input = NULL;
	// free(app_data);
}
