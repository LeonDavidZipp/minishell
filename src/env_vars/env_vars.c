/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 12:59:22 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/14 14:13:25 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// static bool	var_name_valid(char *key, bool includes_equal);
static int	var_name_valid(char *key, bool includes_equal);

// static bool	update_existing_env_var(char *key, char *value, int inc_equal,
// 				char ***env_vars_ptr);
static bool	update_existing_env_var(t_envvar **var, int *exit_code,
				char ***env_vars_ptr);

// char	**update_env_vars(t_envvar **var, int *exit_code, char **env_vars)
// {
// 	char	*new_var;
// 	int		len;

// 	if (update_existing_env_var(var, exit_code, &env_vars))
// 		return (env_vars);
// 	len = ft_null_terminated_arr_len((void **)env_vars);
// 	if (var_name_valid((*var)->key, (*var)->includes_equal))
// 	{
// 		if ((*var)->includes_equal)
// 			new_var = ft_strjoin((*var)->key, "=");
// 		else
// 			new_var = ft_strdup((*var)->key);
// 		env_vars = ft_recalloc(env_vars, len + 2, len, sizeof(char *));
// 		if (!env_vars)
// 			return (free(new_var), NULL);
// 		env_vars[len] = ft_strjoin(new_var, (*var)->value);
// 		free(new_var);
// 	}
// 	else
// 	{
// 		ft_fprintf(2, "%s: export: `%s': %s\n", NAME, (*var)->key, INVALID_ID);
// 		*exit_code = 1;
// 	}
// 	return (env_vars);
// }

char	**update_env_vars(t_envvar **var, int *exit_code, char **env_vars)
{
	char	*new_var;
	int		len;

	if (update_existing_env_var(var, exit_code, &env_vars))
		return (env_vars);
	len = ft_null_terminated_arr_len((void **)env_vars);
	if (var_name_valid((*var)->key, (*var)->includes_equal) == 0)
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
	else if (var_name_valid((*var)->key, (*var)->includes_equal) == 1)
	{
		ft_fprintf(2, "%s: export: `%s': %s\n", NAME, (*var)->key, INVALID_ID);
		*exit_code = 1;
	}
	else if ((*var)->key[0] && (*var)->key[1])
	{
		ft_fprintf(2, "%s: export: `%c%c': %s\n", NAME, (*var)->key[0], (*var)->key[1], INVALID_OP);
		ft_fprintf(2, "export: usage: export [-nf] [name[=value] ...] or export -p\n");
		*exit_code = 2;
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
			ft_fprintf(2, "%s: unset: `%c%c': %s\n", NAME, keys[i][0], keys[i][2], INVALID_OP);
			ft_fprintf(2, "unset: usage: unset [-f] [-v] [name ...]\n");
		}
	}
	ft_free_2d_arr((void **)keys);
	return (exit_code);
}

// static bool	var_name_valid(char *key, bool includes_equal)
// {
// 	int		i;
// 	int		len;

// 	if (!key)
// 		return (false);
// 	if (key[0] == '-')
// 		return (false);
// 	if (!ft_isalpha(key[0]) && key[0] != '_')
// 		return (false);
// 	i = 0;
// 	len = ft_strlen(key);
// 	while (key[++i])
// 	{
// 		if (i == len - 1 && len > 1 && includes_equal && key[i] == '+')
// 			break ;
// 		if (!ft_isalnum(key[i]) && key[i] != '_')
// 			return (false);
// 		else if (key[i] == '=')
// 			return (false);
// 	}
// 	return (true);
// }

static int	var_name_valid(char *key, bool includes_equal)
{
	int		i;
	int		len;

	if (!key)
		return (1);
	if (key[0] == '-')
		return (2);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (1);
	i = 0;
	len = ft_strlen(key);
	while (key[++i])
	{
		if (i == len - 1 && len > 1 && includes_equal && key[i] == '+')
			break ;
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (1);
		else if (key[i] == '=')
			return (1);
	}
	return (0);
}

static bool	update_existing_env_var(t_envvar **var, int *exit_code,
				char ***env_vars_ptr)
{
	int			i;
	char		*new_var;
	char		**env_vars;
	char		*value;

	*exit_code = 0;
	env_vars = *env_vars_ptr;
	i = -1;
	while (env_vars[++i])
	{
		if (ft_strncmp(env_vars[i], (*var)->key, ft_strlen((*var)->key)) == 0
			&& (*var)->includes_equal)
		{
			if ((*var)->includes_plus)
			{
				value = ft_getenv((*var)->key, env_vars);
				value = ft_join_in_place(value, (*var)->value);
			}
			else
				value = ft_strdup((*var)->value);
			free(env_vars[i]);
			new_var = ft_strjoin((*var)->key, "=");
			env_vars[i] = ft_strjoin(new_var, value);
			free(new_var);
			return (true);
		}
		else if (ft_strncmp(env_vars[i], (*var)->key, ft_strlen((*var)->key)) == 0
			&& !(*var)->includes_equal)
			return (true);
	}
	return (false);
}
