/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:06:33 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/25 18:58:50 by intra            ###   ########.fr       */
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
		return ;
	}
	temp = split_envp(var_string);
	if (!temp)
		return ;
	if (ft_null_terminated_arr_len((void **)temp) != 2)
		update_env_vars(temp[0], "", env_vars);
	else
		update_env_vars(temp[0], temp[1], env_vars);
}
