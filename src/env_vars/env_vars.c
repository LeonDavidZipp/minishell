/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 12:59:22 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/11 08:28:20 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	var_name_valid(char *key);

static bool	update_existing_env_var(char *key, char *value, int inc_equal,
				char ***env_vars_ptr);

char	**update_env_vars(t_envvar **var, int *exit_code, char **env_vars)
{
	char	*new_var;
	int		len;

	if (update_existing_env_var((*var)->key, (*var)->value,
			(*var)->includes_equal, &env_vars))
		return (env_vars);
	len = ft_null_terminated_arr_len((void **)env_vars);
	if (var_name_valid((*var)->key))
	{
		if ((*var)->includes_equal)
			new_var = ft_strjoin((*var)->key, "=");
		else
			new_var = ft_strdup((*var)->key);
		env_vars = ft_recalloc(env_vars, len + 2, len, sizeof(char *));
		if (!env_vars)
			return (free(new_var), NULL);
		env_vars[len] = ft_strjoin(new_var, (*var)->value);
		free(new_var);
	}
	else
	{
		ft_fprintf(2, "%s: export: `%s': %s\n", NAME, (*var)->key, INVALID_ID);
		*exit_code = 1;
	}
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
	int		exit_code;

	exit_code = 0;
	keys = ft_split(keys_string, ' ');
	if (!keys)
		return (1);
	i = -1;
	while (keys[++i])
	{
		if (var_name_valid(keys[i]))
			unset_env_var(keys[i], env_vars);
		else
		{
			ft_fprintf(2, "%s: unset: `%s': %s\n", NAME, keys[i], INVALID_ID);
			exit_code = 1;
		}
	}
	ft_free_2d_arr((void **)keys);
	return (exit_code);
}

bool	var_name_valid(char *key)
{
	int		i;

	i = 0;
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (false);
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (false);
		else if (key[i] == '=')
			return (false);
	}
	return (true);
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
		else if (ft_strncmp(env_vars[i], key, ft_strlen(key)) == 0
			&& !inc_equal)
			return (true);
	}
	return (false);
}
