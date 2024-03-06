/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:19:56 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/06 19:21:57 by lzipp            ###   ########.fr       */
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
			printf("found match: %s\n", env_vars[i]);
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

int	main(int argc, char **argv, char **environ)
{
	char	**env_vars;

	(void)argc;
	(void)argv;
	env_vars = init_envp(environ);
	env_vars = update_env_vars("TEST", "test", env_vars);
	env_vars = update_env_vars("TEST2", "test2", env_vars);
	env_vars = update_env_vars("TEST3", "test3", env_vars);
	int i = -1;
	while (env_vars[++i])
	{
		printf("%s\n", env_vars[i]);
	}
	env_vars = unset_env_var("TEST2", env_vars);
	i = -1;
	while (env_vars[++i])
	{
		printf("%s\n", env_vars[i]);
	}
}
