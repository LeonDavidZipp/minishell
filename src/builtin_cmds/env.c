/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:51:31 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/25 15:04:12 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// TODO: implement this when time has come
// you can pass any number of vars to set, e.g. "VAR1=value1 VAR2=value2"
// BUT they will only be set in the local environment, not in the global one
// this means that if you run env VAR1=value1 VAR2=value2, the global
// environment will not be affected, only the child processes will have
// access to these variables

char	**split_env(char *env);

// int	builtin_env(char *new_var, char **env_vars)
// {
// 	char	**var;
// 	char	**split;
// 	int		i;

// 	if (new_var)
// 	{
// 		split = split_env_var_string(new_var, ' ');
// 		if (!split)
// 			return (1);
// 		i = -1;
// 		while (split[++i])
// 		{
// 			var = split_env_var(split[i]);
// 			if (!var)
// 				return (1);
// 			env_vars = update_env_vars(var[0], var[1], env_vars);
// 			if (!env_vars)
// 				return (1);
// 			ft_free_2d_arr((void **)var);
// 		}
// 	}
// 	i = -1;
// 	while (env_vars[++i])
// 	{
// 		if (env_vars[i])
// 			printf("%s\n", env_vars[i]);
// 	}
// 	return (0);
// }

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
	i = -1;
	while (*env_vars[++i])
	{
		if (*env_vars[i] && ft_count_char(*env_vars[i], '=') > 0)
			printf("%s\n", *env_vars[i]);
	}
	return (0);
}

// char	**split_env(char *envp)
// {
// 	char		**key_value;
// 	int			len;
// 	key_value = ft_calloc(3, sizeof(char *));
// 	if (!key_value)
// 		return (NULL);
// 	len = 0;
// 	while (envp[len] && envp[len] != '=')
// 	{
// 		if (ft_isspace(envp[len]))
// 		{
// 			free(key_value);
// 			return (NULL);
// 		}
// 		len++;
// 	}
// 	if (ft_isspace(envp[len + 1]))
// 	{
// 		free(key_value);
// 		return (NULL);
// 	}
// 	key_value[0] = ft_substr(envp, 0, len);
// 	key_value[1] = ft_substr(envp, len + 1, ft_strlen(envp) - len);
// 	return (key_value);
// }
