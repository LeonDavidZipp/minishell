/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:45:28 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/19 11:25:45 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*increment_shlvl(char **shlvl);

char	**init_envp(char **env_vars)
{
	int			i;
	char		**envp;

	envp = ft_calloc(ft_null_terminated_arr_len((void **)env_vars) + 1,
			sizeof(char *));
	if (!envp)
		return (NULL);
	i = -1;
	while (env_vars[++i])
	{
		if (ft_strncmp(env_vars[i], "SHLVL=", 6) == 0)
			envp[i] = increment_shlvl(&(env_vars[i]));
		else
			envp[i] = ft_strdup(env_vars[i]);
	}
	return (envp);
}

static char	*increment_shlvl(char **shlvl)
{
	int		shlvl_int;
	char	*new_shlvl;
	char	*shlvl_result;

	if (ft_str_isdigit(*shlvl + 6))
	{
		shlvl_int = ft_atoi(*shlvl + 6);
		new_shlvl = ft_itoa(shlvl_int + 1);
		shlvl_result = ft_strjoin("SHLVL=", new_shlvl);
		free(new_shlvl);
	}
	else
		shlvl_result = ft_strdup("SHLVL=1");
	return (shlvl_result);
}

t_envvar	*fill_result(t_envvar *result, char *envp)
{
	result->key = ft_strdup(envp);
	result->value = NULL;
	result->includes_equal = false;
	result->includes_plus = false;
	return (result);
}

t_envvar	*fill_result2(t_envvar *result, char *envp, int len)
{
	if (result->includes_plus == false)
		result->key = ft_substr(envp, 0, len);
	else
		result->key = ft_substr(envp, 0, len - 1);
	result->value = NULL;
	result->includes_equal = (envp[len] && envp[len] == '=');
	if (envp[len] && envp[len] == '=' && !envp[len + 1])
		result->value = ft_strdup("");
	else if (envp[len] && envp[len] == '=' && envp[len + 1])
		result->value = ft_substr(envp, len + 1, ft_strlen(envp) - len);
	return (result);
}
