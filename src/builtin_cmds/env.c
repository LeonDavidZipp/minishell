/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:51:31 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/25 17:13:27 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_env(char *var_string, char ***env_vars)
{
	t_envvar	**vars;
	int			i;

	if (var_string)
	{
		vars = split_env_vars(var_string);
		if (!vars)
			return (1);
		i = -1;
		while (vars[++i])
		{
			*env_vars = update_env_vars(vars[i]->key, vars[i]->value,
				vars[i]->includes_equal, *env_vars);
		}
		free_vars(vars);
	}
	i = 0;
	while ((*env_vars)[i])
	{
		if ((*env_vars)[i] && ft_count_char((*env_vars)[i], '=') > 0)
			printf("%s\n", (*env_vars)[i]);
		i++;
	}
	return (0);
}
