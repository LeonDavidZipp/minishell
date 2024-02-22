/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:06:33 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/22 12:56:36 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_export(t_env_var **env_vars, char *var_string)
{
	char	**temp;

	if (!var_string)
	{
		while (*env_vars)
		{
			if ((*env_vars)->value)
				printf("declare -x %s=\"%s\"\n", (*env_vars)->key,
					(*env_vars)->value);
			else
				printf("declare -x %s\n", (*env_vars)->key);
			env_vars++;
		}
	}
	temp = split_environ(var_string);
	if (!temp)
		return ;
	if (ft_null_terminated_arr_len((void **)temp) != 2)
		update_env_vars(temp[0], "", env_vars);
	else
		update_env_vars(temp[0], temp[1], env_vars);
}
