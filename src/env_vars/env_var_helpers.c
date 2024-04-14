/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:45:28 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/14 16:23:18 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
		envp[i] = ft_strdup(env_vars[i]);
	return (envp);
}

int	var_name_valid(char *key, bool includes_equal)
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

void	handle_non_zero(char **key, int *exit_code)
{
	if (*exit_code == 1)
		ft_fprintf(2, "%s: export: `%s': %s\n", NAME, *key, INVALID_ID);
	else if ((*key)[0] && (*key)[1] && *exit_code == 2)
	{
		ft_fprintf(2, "%s: export: `%c%c': %s\n%s%s", NAME, (*key)[0],
			(*key)[1], INVALID_OP, EXPORT_USG, EXPORT_USG2);
	}
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
