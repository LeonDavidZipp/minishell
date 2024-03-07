/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:06:33 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/06 18:58:45 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_vars(t_env_var *env_vars);

void	builtin_export(char *var_string, t_env_var **env_vars)
{
	char	**vars;
	char	**temp;
	int		i;

	if (!var_string)
	{
		print_vars(*env_vars);
		return ;
	}
	vars = ft_split(var_string, ' ');
	if (!vars)
		return ;
	i = -1;
	while (vars[++i])
	{
		temp = split_envp(vars[i]);
		if (!temp)
			continue ;
		if (ft_null_terminated_arr_len((void **)temp) != 2)
			update_env_vars(temp[0], "", env_vars);
		else
			update_env_vars(temp[0], temp[1], env_vars);
		free(temp);
	}
	ft_free_2d_arr((void **)vars);
}

static void	print_vars(t_env_var *env_vars)
{
	while (env_vars)
	{
		if (env_vars->value)
			printf("declare -x %s=\"%s\"\n", env_vars->key, env_vars->value);
		else
			printf("declare -x %s\n", env_vars->key);
		env_vars = env_vars->next;
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
// 	// Add two new variables to the environment
// 	builtin_export("VAR1=value1 VAR2=value2", &env_vars);
// 	printf("\n----------------\n");
// 	builtin_export(NULL, &env_vars);
// 	return 0;
// }
