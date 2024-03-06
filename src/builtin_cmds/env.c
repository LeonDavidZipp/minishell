/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:51:31 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/06 18:58:51 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// TODO: implement this when time has come
// you can pass any number of vars to set, e.g. "VAR1=value1 VAR2=value2"
// BUT they will only be set in the local environment, not in the global one
// this means that if you run env VAR1=value1 VAR2=value2, the global
// environment will not be affected, only the child processes will have
// access to these variables

void	builtin_env(char *new_var, t_env_var **env_vars)
{
	char		**key_value;
	t_env_var	*temp;

	if (new_var)
	{
		key_value = split_envp(new_var);
		if (!(key_value[1]))
			update_env_vars(key_value[0], "", env_vars);
		else
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
