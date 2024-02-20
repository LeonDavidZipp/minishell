/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:05:35 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/20 11:18:59 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	match(char *pattern, char *string)
{
	while (*pattern && *string)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			while (*string)
			{
				if (match(pattern, string))
					return (true);
				string++;
			}
			return (match(pattern, string));
		}
		else if (*pattern == *string)
		{
			pattern++;
			string++;
		}
		else
			return (false);
	}
	return (*pattern == *string);
}

int	env_var_size(char *input, int *i)
{
	int		start;
	int		size;
	char	*name;
	char	*value;

	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	name = ft_substr(input, start, *i - start);
	if (!name)
		return (0);
	value = getenv(name);
	if (value)
		size = ft_strlen(value);
	else
		size = 0;
	free(name);
	return (size);
}

int	get_new_size(char *input, int last_exit_code)
{
	int	i;
	int	size;
	int	start;

	i = 0;
	size = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '?')
		{
			size += ft_dec_len(last_exit_code);
			i += 2;
		}
		else if (input[i] == '$')
		{
			i++;
			size += env_var_size(input, &i);
		}
		else
		{
			size++;
			i++;
		}
	}
	return (size);
}
