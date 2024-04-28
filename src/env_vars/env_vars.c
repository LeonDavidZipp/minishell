/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 12:59:22 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/28 14:50:29 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool		update_existing_env_var(t_envvar **var, int *exit_code,
					char ***env_vars_ptr);
static char		*determine_value(t_envvar **var, char ***env_vars_ptr);

char	**update_env_vars(t_envvar **var, int *exit_code, char **env_vars)
{
	char	*new_var;
	int		len;

	if (update_existing_env_var(var, exit_code, &env_vars))
		return (env_vars);
	len = ft_null_terminated_arr_len((void **)env_vars);
	*exit_code = var_name_valid((*var)->key, (*var)->includes_equal);
	if (*exit_code == 0)
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
		handle_non_zero(&(*var)->key, exit_code);
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

int	unset_multiple_env_vars(char *keys_string, char ***env_vars)
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
		exit_code = var_name_valid(keys[i], false);
		if (exit_code == 0)
			unset_env_var(keys[i], env_vars);
		else if (exit_code == 1)
			ft_fprintf(2, "%s: unset: `%s': %s\n", NAME, keys[i], INVALID_ID);
		else if (keys[i][0] && keys[i][1] && exit_code == 2)
		{
			ft_fprintf(2, "%s: unset: `%c%c': %s\n%s", NAME, keys[i][0],
				keys[i][2], INVALID_OP, UNSET_USG);
			break ;
		}
	}
	ft_free_2d_arr((void **)keys);
	return (exit_code);
}

static bool	update_existing_env_var(t_envvar **var, int *exit_code,
				char ***env_vars_ptr)
{
	int			i;
	char		*new_var;
	char		**env;
	char		*value;

	*exit_code = 0;
	env = *env_vars_ptr;
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], (*var)->key, ft_strlen((*var)->key)) == 0
			&& (*var)->includes_equal && env[i][ft_strlen((*var)->key)] == '=')
		{
			value = determine_value(var, env_vars_ptr);
			free(env[i]);
			new_var = ft_strjoin((*var)->key, "=");
			env[i] = ft_strjoin(new_var, value);
			return (free(value), free(new_var), true);
		}
		else if (ft_strncmp(env[i], (*var)->key,
				ft_strlen((*var)->key)) == 0 && !(*var)->includes_equal
			&& env[i][ft_strlen((*var)->key)] == '=')
			return (true);
	}
	return (false);
}

static char	*determine_value(t_envvar **var, char ***env_vars_ptr)
{
	char		*value;
	char		**env_vars;

	env_vars = *env_vars_ptr;
	if ((*var)->includes_plus)
	{
		value = ft_getenv((*var)->key, env_vars);
		value = ft_join_in_place(value, (*var)->value);
	}
	else
		value = ft_strdup((*var)->value);
	return (value);
}
