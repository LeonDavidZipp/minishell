/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_multiple_envp.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:29:04 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/08 17:20:19 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	count_substr(char *s, char c);
char		**split_env_var_string(char *envp, char c);
char		**split_env_var(char *envp);
static char	*ft_split_str(const char *s, char c, int *i);

char	***split_env_vars(char *envp)
{
	// char	***env_vars;
	char	**keys_values;
	char	**temp;
	int		i;
	int		j;

	keys_values = split_env_var_string(envp, ' ');
	if (!keys_values)
		return (NULL);
	// i = ft_null_terminated_arr_len((void **)keys_values);
	i = -1;
	while (keys_values[++i])
	{
		j = -1;
		printf("keys_values[%d]: |%s|\n", i, keys_values[i]);
		temp = split_env_var(keys_values[i]);
		printf("done\n");
		while (temp[++j])
			printf("temp[%d]: %s\n", j, temp[j]);
		printf("\n----------------\n");
	}
	return (NULL);
}

char	**split_env_var(char *envp)
{
	char		**key_value;
	int			len;
	key_value = ft_calloc(3, sizeof(char *));
	if (!key_value)
		return (NULL);
	len = 0;
	printf("before while");
	while (envp[len] && envp[len] != '=')
	{
		if (ft_isspace(envp[len]))
			return (free(key_value), NULL);
		len++;
	}
	printf("len: %d\n", len);
	if (ft_isspace(envp[len + 1]))
		return (free(key_value), NULL);
	printf("len: %d\n", len);
	key_value[0] = ft_substr(envp, 0, len);
	key_value[1] = ft_substr(envp, len + 1, ft_strlen(envp) - len);
	key_value[1] = ft_ntrim_in_place(key_value[1], "\"\'", 1);
	return (key_value);
}

char	**split_env_var_string(char *envp, char c)
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

#include <stdio.h>
int main(void)
{
	char ***env_vars;
	char *envp = "KEY1= KEY2=VALUE2 KEY3=\" hey\" KEY4=        hi";
	int i = 0;
	env_vars = split_env_vars(envp);
	while (env_vars[i])
	{
		printf("|Key: %s, Value: %s|\n", env_vars[i][0], env_vars[i][1]);
		i++;
	}
	return 0;
}
