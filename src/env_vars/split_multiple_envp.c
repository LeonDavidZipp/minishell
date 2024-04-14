/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_multiple_envp.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:29:04 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/14 16:18:49 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int		count_substr(char *s, char c);
static char		**split_env_var_string(char *envp, char c);
static char		*ft_split_str(const char *s, char c, int *i);

t_envvar	**split_env_vars(char *envp, t_app_data **app_data)
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
	{
		keys_values[len] = expand_and_remove_in_place(keys_values[len],
			(*app_data)->last_exit_code, (*app_data)->env_vars, 0);
		env_vars[len] = split_env_var(keys_values[len]);
	}
	ft_free_2d_arr((void **)keys_values);
	return (env_vars);
}

static t_envvar	*fill_result2(t_envvar *result, char *envp, int len);
static t_envvar	*fill_result(t_envvar *result, char *envp);

t_envvar	*split_env_var(char *envp)
{
	t_envvar	*result;
	int			len;

	result = (t_envvar *)malloc(sizeof(t_envvar));
	if (!envp || !result)
		return (NULL);
	if (envp[0] == '=' || envp[0] == '+')
		return(fill_result(result, envp));
	len = 0;
	result->includes_plus = false;
	while (envp[len] && envp[len] != '=')
	{
		if (envp[len] == '+' && envp[len + 1] && envp[len + 1] == '=')
			result->includes_plus = true;
		else if (envp[len] == '+')
			return(fill_result(result, envp));
		len++;
	}
	return (fill_result2(result, envp, len));
}

static t_envvar	*fill_result(t_envvar *result, char *envp)
{
	result->key = ft_strdup(envp);
	result->value = NULL;
	result->includes_equal = false;
	result->includes_plus = false;
	return (result);
}

static t_envvar	*fill_result2(t_envvar *result, char *envp, int len)
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
		if ((s[i] == '\"' && !quote) && (i == 0 || !dquote))
			count++;
		if (s[i] == '\"' && !quote)
			dquote = !dquote;
		if ((s[i] == '\'' && !dquote) && (i == 0 || !quote))
			count++;
		if (s[i] == '\'' && !dquote)
			quote = !quote;
		if (!quote && !dquote
			&& ((i == 0 && s[i] != c) || (s[i] != c && s[i - 1] == c)))
			count++;
	}
	return (count);
}
