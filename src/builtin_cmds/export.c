/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:06:33 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/16 16:05:07 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// need a list of environment variables tah were exported this session

void	builtin_export(t_env_var **env_vars, char *var_string)
{
	char	*temp;

	if (!var_string)
	{
		// print output all env vars exported this session
		return ;
	}
	temp = ft_split(var_string, "=");
	if (ft_null_terminated_arr_len(temp) != 2)
		update_env_vars(temp[0], "", env_vars);
	else
		update_env_vars(temp[0], temp[1], env_vars);
	// check if var_string is valid
	// if not, print error message
	// else, add var_string to env_vars
}
