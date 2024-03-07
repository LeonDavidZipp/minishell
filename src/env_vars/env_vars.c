/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:19:56 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/07 12:38:48 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**update_env_vars(char *key, char *value, char **env_vars)
{
	int		i;
	char	*new_var;

	i = -1;
	while (env_vars[++i])
	{
		if (ft_strncmp(env_vars[i], key, ft_strlen(key)) == 0)
		{
			free(env_vars[i]);
			new_var = ft_strjoin(key, "=");
			env_vars[i] = ft_strjoin(new_var, value);
			free(new_var);
			return (env_vars);
		}
	}
	new_var = ft_strjoin(key, "=");
	env_vars = ft_recalloc(env_vars,
			ft_null_terminated_arr_len((void **)env_vars) + 2,
			sizeof(char *));
	if (!env_vars || !env_vars)
		return (NULL);
	env_vars[i] = ft_strjoin(new_var, value);
	free(new_var);
	return (env_vars);
}

char	**unset_env_var(char *key, char **env_vars)
{
	int		i;

	i = -1;
	while (env_vars[++i])
	{
		if (ft_strncmp(env_vars[i], key, ft_strlen(key)) == 0)
		{
			while (env_vars[i])
			{
				free(env_vars[i]);
				env_vars[i] = env_vars[i + 1];
				i++;
			}
			break ;
		}
	}
	return (env_vars);
}

char	**init_envp(char **env_vars)
{
	int			i;
	char		**envp;

	i = -1;
	envp = ft_calloc(ft_null_terminated_arr_len((void **)env_vars) + 1,
			sizeof(char *));
	if (!envp)
		return (NULL);
	while (env_vars[++i])
		envp[i] = ft_strdup(env_vars[i]);
	return (envp);
}
