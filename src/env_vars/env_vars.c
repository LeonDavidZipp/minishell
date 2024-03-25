/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:25:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/25 17:47:20 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	var_name_valid(char *key);
static bool	update_existing_env_var(char *key, char *value, int inc_equal,
				char ***env_vars_ptr);

char	**update_env_vars(char *key, char *value, bool inc_equal,
				char **env_vars)
{
	char	*new_var;
	int		len;

	if (update_existing_env_var(key, value, inc_equal, &env_vars))
		return (env_vars);
	len = ft_null_terminated_arr_len((void **)env_vars);
	if (var_name_valid(key))
	{
		if (inc_equal)
			new_var = ft_strjoin(key, "=");
		else
			new_var = ft_strdup(key);
		env_vars = ft_recalloc(env_vars, len + 2, sizeof(char *));
		if (!env_vars)
			return (NULL);
		env_vars[len] = ft_strjoin(new_var, value);
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
	return (0);
}

static bool	update_existing_env_var(char *key, char *value, int inc_equal,
				char ***env_vars_ptr)
{
	int			i;
	char		*new_var;
	char		**env_vars;

	env_vars = *env_vars_ptr;
	i = -1;
	while (env_vars[++i])
	{
		if (ft_strncmp(env_vars[i], key, ft_strlen(key)) == 0 && inc_equal)
		{
			free(env_vars[i]);
			new_var = ft_strjoin(key, "=");
			env_vars[i] = ft_strjoin(new_var, value);
			free(new_var);
			return (true);
		}
	}
	return (false);
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
