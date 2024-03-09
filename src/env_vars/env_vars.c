/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/03/09 20:37:33 by lzipp            ###   ########.fr       */
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

// char	**unset_env_var(char *key, char **env_vars)
// {
// 	int		i;

// 	i = -1;
// 	while (env_vars[++i])
// 	{
// 		if (ft_strncmp(env_vars[i], key, ft_strlen(key)) == 0)
// 		{
// 			while (env_vars[i])
// 			{
// 				free(env_vars[i]);
// 				env_vars[i] = env_vars[i + 1];
// 				i++;
// 			}
// 			break ;
// 		}
// 	}
// 	return (env_vars);
// }

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

void	unset_env_vars(char *keys_string, char ***env_vars)
	{
	char	**keys;
	int		i;

	keys = ft_split(keys_string, ' ');
	if (!keys)
		return ;
	i = -1;
	while (keys[++i])
	{
		unset_env_var(keys[i], env_vars);
	}
	free(keys);
	i = -1;
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
// }
