/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:05:35 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 14:00:50 by cgerling         ###   ########.fr       */
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

int	calc_wildcard_size(DIR *dir, char *pattern, int position)
{
	struct dirent	*entry;
	int				size;
	bool			flag;

	flag = false;
	size = 0;
	entry = readdir(dir);
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
	return (size);
}

int	wildcard_size(char *input, int *i)
{
	char			*pattern;
	int				position;
	int				size;
	DIR				*dir;

	pattern = get_pattern(input, i, &position);
	if (!pattern)
		return (0);
	dir = opendir(".");
	if (dir == NULL)
		return (0);
	size = calc_wildcard_size(dir, pattern, position);
	closedir(dir);
	free(pattern);
	return (size);
}

int	calculate_size(char *input, int *i, int last_exit, bool *quotes)
{
	int		size;

	size = 0;
	handle_quotes(input[*i], &quotes[0], &quotes[1]);
	if (input[*i] == '$' && input[*i + 1] == '?' && !quotes[0])
	{
		size += ft_dec_len(last_exit);
		*i += 2;
	}
	else if (input[*i] == '$' && !quotes[0])
	{
		(*i)++;
		size += env_var_size(input, i);
	}
	else if (input[*i] == '*' && !quotes[0] && !quotes[1])
		size += wildcard_size(input, i) - 1;
	else
	{
		size++;
		(*i)++;
	}
	return (size);
}

int	get_new_size(char *input, int last_exit_code)
{
	int		i;
	int		size;
	bool	quotes[2];

	i = 0;
	size = 0;
	quotes[0] = false;
	quotes[1] = false;
	while (input[i])
	{
		size += calculate_size(input, &i, last_exit_code, quotes);
	}
	return (size);
}
