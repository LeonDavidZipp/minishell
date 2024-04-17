/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:51:31 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/12 12:55:11 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_env(char *var_string, char ***env_vars)
{
	int			i;
	int			exit_code;

	exit_code = 0;
	if (var_string)
		return (ft_fprintf(2, "env: No arguments allowed\n"), 1);
	i = 0;
	while ((*env_vars)[i])
	{
		if ((*env_vars)[i] && ft_count_char((*env_vars)[i], '=') > 0)
			printf("%s\n", (*env_vars)[i]);
		i++;
	}
	return (exit_code);
}
