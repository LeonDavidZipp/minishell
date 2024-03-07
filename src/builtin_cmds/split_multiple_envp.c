/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_multiple_envp.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:29:04 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/07 16:02:09 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	count_substr(char *s, char c)
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
		else if (!quote && !dquote
			&& ((i == 0 && s[i] != c) || (s[i] != c && s[i - 1] == c)))
			count++;
	}
	return (count);
}

// example string: "VAR1=value1 VAR2=value2"
char	**split_env_var_string(char *envp)
{
	int		begin;
	int		end;
	char	*temp;
	char	***split;
	bool	dquote;
	bool	quote;

	split = ft_calloc(2, sizeof(char **));
	if (!split || !envp)
		return (NULL);
	begin = -1;
	while (envp[++begin])
	{
		
	}
}

char	**split_env(char *envp)
{
	char		**key_value;
	int			len;
	key_value = ft_calloc(3, sizeof(char *));
	if (!key_value)
		return (NULL);
	len = 0;
	while (envp[len] && envp[len] != '=')
	{
		if (ft_isspace(envp[len]))
			return (free(key_value), NULL);
		len++;
	}
	if (ft_isspace(envp[len + 1]))
		return (free(key_value), NULL);
	key_value[0] = ft_substr(envp, 0, len);
	key_value[1] = ft_substr(envp, len + 1, ft_strlen(envp) - len);
	key_value[1] = ft_ntrim_in_place(key_value[1], "\"\'", 1);
	return (key_value);
}

// int	main(void)
// {
// 	char	**key;
// 	char	*envp;

// 	envp = "VAR1=\"      value1\"";
// 	key = split_env(envp);
// 	if (!key)
// 		return (1);
// 	for (int j = 0; j < 1; j++)
// 		printf("key: %s, value:%s\n", key[0], key[1]);
		
// }
