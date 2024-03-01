/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:05:35 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/29 17:05:26 by cgerling         ###   ########.fr       */
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

char	*get_pattern(char *input, int *i, int *position)
{
	int		start;
	int		end;
	int		tmp;
	char	*pattern;

	tmp = *i;
	start = 0;
	end = 0;
	while (*i >= 0 && !is_space(input[*i]))
		(*i)--;
	start = *i + 1;
	*position = tmp - start;
	*i = tmp;
	while (input[*i] && !is_space(input[*i]))
		(*i)++;
	end = *i;
	pattern = ft_substr(input, start, end - start);
	if (!pattern)
		return (NULL);
	return (pattern);
}

int	wildcard_size(char *input, int *i)
{
	char			*pattern;
	int				position;
	DIR				*dir;
	struct dirent	*entry;
	int				size;
	bool			flag;

	flag = false;
	pattern = get_pattern(input, i, &position);
	if (!pattern)
		return (0);
	dir = opendir(".");
	if (dir == NULL)
		return (0);
	entry = readdir(dir);
	size = 0;
	while (entry != NULL)
	{
		if (match(pattern, entry->d_name))
		{
			if (!flag)
			{
				size -= position;
				flag = true;
			}
			size += ft_strlen(entry->d_name) + 1;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	free(pattern);
	return (size);
}

int	get_new_size(char *input, int last_exit_code)
{
	int	i;
	int	size;
	bool	s_quote;
	bool	d_quote;

	i = 0;
	size = 0;
	s_quote = false;
	d_quote = false;
	while (input[i])
	{
		handle_quotes(input[i], &s_quote, &d_quote);
		if (input[i] == '$' && input[i + 1] == '?' && !s_quote)
		{
			size += ft_dec_len(last_exit_code);
			i += 2;
		}
		else if (input[i] == '$' && !s_quote)
		{
			i++;
			size += env_var_size(input, &i);
		}
		else if (input[i] == '*' && !s_quote && !d_quote)
			size += wildcard_size(input, &i) - 1;
		else
		{
			size++;
			i++;
		}
	}
	return (size);
}
