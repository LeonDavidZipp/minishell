/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:51:31 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 07:05:30 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_env(char *new_var, t_env_var **env_vars)
{
	char		**key_value;
	t_env_var	*temp;

	if (new_var)
	{
		key_value = split_envp(new_var);
		update_env_vars(key_value[0], key_value[1], env_vars);
		ft_free_2d_arr((void **)key_value);
	}
	temp = *env_vars;
	while (temp)
	{
		if ((temp)->value)
			printf("%s=%s\n", (temp)->key, (temp)->value);
		temp = (temp)->next;
	}
}

// int main()
// {
// 	t_env_var	*env_vars;

// 	env_vars = NULL;
// 	update_env_vars("key1", "value1", &env_vars);
// 	update_env_vars("key2", "value2", &env_vars);
// 	update_env_vars("key3", "value3", &env_vars);
// 	update_env_vars("key4", "value4", &env_vars);
//     // Add two new variables to the environment
//     builtin_env("VAR1=value1", &env_vars);
// 	printf("\n----------------\n");
//     builtin_env("VAR2=value2", &env_vars);
// 	printf("\n----------------\n");
// 	builtin_env(NULL, &env_vars);
// 	return 0;
// }
