/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_multiple_envp.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:29:04 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/11 13:46:30 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int		count_substr(char *s, char c);
static char		**split_env_var_string(char *envp, char c);
static char		*ft_split_str(const char *s, char c, int *i);

char	***split_env_vars(char *envp)
{
	char	***env_vars;
	char	**keys_values;
	int		len;

	keys_values = split_env_var_string(envp, ' ');
	if (!keys_values)
		return (NULL);
	len = ft_null_terminated_arr_len((void **)keys_values);
	env_vars = ft_calloc(len + 1, sizeof(char **));
	if (!env_vars)
		return (ft_free_2d_arr((void **)keys_values), NULL);
	len = -1;
	while (keys_values[++len])
		env_vars[len] = split_env_var(keys_values[len]);
	return (env_vars);
}

char	**split_env_var(char *envp)
{
	char	**result;
	int		len1;

	result = ft_calloc(3, sizeof(char *));
	if (!envp || !result)
		return (NULL);
	len1 = 0;
	while (envp[len1] && envp[len1] != '=')
	{
		if (ft_isspace(envp[len1]))
			return (ft_free_2d_arr((void **)result), NULL);
		len1++;
	}
	result[0] = ft_substr(envp, 0, len1);
	if (envp[len1] && envp[len1] == '=' && !envp[len1 + 1])
		result[1] = ft_strdup("");
	else if (envp[len1] && envp[len1] == '=' && envp[len1 + 1])
	{
		result[1] = ft_substr(envp, len1 + 1, ft_strlen(envp) - len1);
		result[1] = ft_ntrim_in_place(result[1], "\"\'", 1);
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
