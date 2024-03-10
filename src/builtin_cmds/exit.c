/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:53:27 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/10 11:31:41 by lzipp            ###   ########.fr       */
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

static void	free_app_data(t_app_data *app)
{
	ft_free_2d_arr((void **)app->env_vars);
	free(app);
}
