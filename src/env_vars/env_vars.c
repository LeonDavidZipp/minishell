/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/03/25 16:53:21 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	var_name_valid(char *key);

char	**update_env_vars(char *key, char *value, bool inc_equal,
				char **env_vars)
{
	int		i;
	char	*new_var;

	i = -1;
	printf("before update key: %s\n", key);
	while (env_vars[++i])
	{
		
		printf("i: %d, str: %s\n", i, env_vars[i]);
		// if (ft_strncmp(env_vars[i], key, ft_strlen(key)) == 0 && inc_equal)
		// {
		// 	printf("in if\n");
		// 	free(env_vars[i]);
		// 	new_var = ft_strjoin(key, "=");
		// 	env_vars[i] = ft_strjoin(new_var, value);
		// 	free(new_var);
		// 	return (env_vars);
		// }
	}
	printf("before adding new var\n");
	if (var_name_valid(key))
	{
		if (inc_equal)
			new_var = ft_strjoin(key, "=");
		else
			new_var = ft_strdup(key);
		env_vars = ft_recalloc(env_vars,
				ft_null_terminated_arr_len((void **)env_vars) + 2,
				sizeof(char *));
		if (!env_vars)
			return (NULL);
		env_vars[i] = ft_strjoin(new_var, value);
		free(new_var);
	}
	else
		ft_fprintf(2, "%s: export: `%s': not a valid identifier\n", NAME, key);
	return (env_vars);
}

void	unset_env_var(char *key, char ***env_vars)
{
	int		i;
	int		j;

	i = -1;
	while ((*env_vars)[++i])
	{
		if (ft_strncmp((*env_vars)[i], key, ft_strlen(key)) == 0)
		{
			free((*env_vars)[i]);
			j = i;
			while ((*env_vars)[j])
			{
				(*env_vars)[j] = (*env_vars)[j + 1];
				j++;
			}
			break ;
		}
	}
}

int	unset_env_vars(char *keys_string, char ***env_vars)
{
	char	**keys;
	int		i;

	keys = ft_split(keys_string, ' ');
	if (!keys)
		return (1);
	i = -1;
	while (keys[++i])
	{
		unset_env_var(keys[i], env_vars);
	}
	ft_free_2d_arr((void **)keys);
	i = -1;
	return (0);
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

static bool	var_name_valid(char *key)
{
	int		i;

	i = 0;
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (false);
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (false);
	}
	return (true);
}

// int main(void)
// {
// 	char	**env_vars;
// 	char	**envp;
// 	char	*key_string;

// 	env_vars = ft_split("KEY1=VALUE1 KEY2=VALUE2 KEY4=hi", ' ');
// 	envp = init_envp(env_vars);
// 	int i = -1;
// 	while (envp[++i])
// 		printf("%s\n", envp[i]);
// 	printf("\n----------------\n");
// 	envp = update_env_vars("KEY1", "NEW_VALUE1", envp);
// 	i = -1;
// 	while (envp[++i])
// 		printf("%s\n", envp[i]);
// 	printf("\n----------------\n");
// 	key_string = ft_strdup("KEY1 KEY3 hi gay KEY4");
// 	unset_env_vars(key_string, &envp);
// 	i = -1;
// 	while (envp[++i])
// 		printf("%s\n", envp[i]);
// 	ft_free_2d_arr((void **)env_vars);
// 	ft_free_2d_arr((void **)envp);
// 	free(key_string);
// 	return (0);
// }
