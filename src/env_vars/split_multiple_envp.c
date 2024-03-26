/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_multiple_envp.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:29:04 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/26 14:11:47 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int		count_substr(char *s, char c);
static char		**split_env_var_string(char *envp, char c);
static char		*ft_split_str(const char *s, char c, int *i);

t_envvar	**split_env_vars(char *envp)
{
	t_envvar	**env_vars;
	char		**keys_values;
	int			len;

	keys_values = split_env_var_string(envp, ' ');
	if (!keys_values)
		return (NULL);
	len = ft_null_terminated_arr_len((void **)keys_values);
	env_vars = ft_calloc((len + 1), sizeof(t_envvar *));
	if (!env_vars)
		return (ft_free_2d_arr((void **)keys_values), NULL);
	len = -1;
	while (keys_values[++len])
		env_vars[len] = split_env_var(keys_values[len]);
	return (env_vars);
}

t_envvar	*split_env_var(char *envp)
{
	t_envvar	*result;
	int			len1;

	result = (t_envvar *)malloc(sizeof(t_envvar));
	if (!envp || !result)
		return (NULL);
	len1 = 0;
	if (envp[len1] == '=')
		len1++;
	while (envp[len1] && envp[len1] != '=')
		len1++;
	result->key = ft_substr(envp, 0, len1);
	result->value = NULL;
	result->includes_equal = (envp[len1] && envp[len1] == '=');
	if (envp[len1] && envp[len1] == '=' && !envp[len1 + 1])
		result->value = ft_strdup("");
	else if (envp[len1] && envp[len1] == '=' && envp[len1 + 1])
	{
		result->value = ft_substr(envp, len1 + 1, ft_strlen(envp) - len1);
		result->value = ft_ntrim_in_place(result->value, "\"\'", 1);
	}
	return (result);
}

static char	**split_env_var_string(char *envp, char c)
{
	int		str_count;
	char	**result;
	int		i;
	int		j;

	str_count = count_substr(envp, c);
	result = ft_calloc(str_count + 1, sizeof(char *));
	if (!result)
		return (NULL);
	i = -1;
	j = 0;
	while (envp[++i] && j < str_count)
	{
		if (envp[i] != c)
		{
			result[j] = ft_split_str(envp, c, &i);
			if (!result[j++])
				return (ft_free_2d_arr((void **)result), NULL);
		}
	}
	return (result);
}

static char	*ft_split_str(const char *s, char c, int *i)
{
	int		len;
	bool	dquote;
	bool	quote;

	len = *i;
	dquote = false;
	quote = false;
	while (s[*i] && (s[*i] != c || dquote || quote))
	{
		if (s[*i] == '"' && !quote)
			dquote = !dquote;
		else if (s[*i] == '\'' && !dquote)
			quote = !quote;
		(*i)++;
	}
	(*i)--;
	return (ft_substr(s, len, *i - len + 1));
}

static int	count_substr(char *s, char c)
{
	int		i;
	int		count;
	bool	dquote;
	bool	quote;

	i = -1;
	count = 0;
	dquote = false;
	quote = false;
	while (s[++i])
	{
		if (s[i] == '\"' && !quote)
			dquote = !dquote;
		else if (s[i] == '\'' && !dquote)
			quote = !quote;
		if (!quote && !dquote
			&& ((i == 0 && s[i] != c) || (s[i] != c && s[i - 1] == c)))
			count++;
	}
	return (count);
}
