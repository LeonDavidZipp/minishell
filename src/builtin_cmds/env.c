/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:51:31 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/07 14:29:55 by lzipp            ###   ########.fr       */
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

void	builtin_env(char *new_var, char **env_vars)
{
	char	**key;
	char	**split;
	int		i;
	
	if (new_var)
	{
		split = ft_split(new_var, ' ');
		if (!split)
			return ;
		i = -1;
		while (split[++i])
		{
			key = split_env(split[i]);
			if (!key)
				return ;
			for (int j = 0; j < 2; j++)
				printf("key%d: %s\n", j, key[j]);
			env_vars = update_env_vars(key[0], key[1], env_vars);
			if (!env_vars)
				return ;
			ft_free_2d_arr((void **)key);
		}
	}
	i = -1;
	while (env_vars[++i])
	{
		if (env_vars[i])
			printf("%s\n", env_vars[i]);
	}
	ft_free_2d_arr((void **)env_vars); // need to check freeing and leaking
}

char	**split_env(char *envp)
{
	char		**key_value;
	int			len;
	key_value = ft_calloc(3, sizeof(char *));
	if (!key_value)
		return (NULL);
	len = 0;
	while (envp[len] && envp[len] != '=')
	{
		if (ft_isspace(envp[len]))
		{
			free(key_value);
			return (NULL);
		}
		len++;
	}
	if (ft_isspace(envp[len + 1]))
	{
		free(key_value);
		return (NULL);
	}
	key_value[0] = ft_substr(envp, 0, len);
	key_value[1] = ft_substr(envp, len + 1, ft_strlen(envp) - len);
	return (key_value);
}