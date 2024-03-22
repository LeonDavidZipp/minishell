/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:05:35 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/22 14:50:35 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	env_var_size(char *input, int *i)
{
	int		start;
	int		size;
	char	*name;
	char	*value;

	start = i[0];
	while (ft_isalnum(input[i[0]]) || input[i[0]] == '_')
		i[0]++;
	name = ft_substr(input, start, i[0] - start);
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

	pattern = get_pattern(input, &i[0], &position);
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

int	calculate_size(char *input, int *i, int exit_code, bool *quotes)
{
	int		size;

	size = 0;
	handle_quotes(input[i[0]], &quotes[0], &quotes[1]);
	if (input[i[0]] == '$' && input[i[0] + 1] == '?' && !quotes[0])
	{
		size += ft_dec_len(exit_code);
		i[0] += 2;
	}
	else if (input[i[0]] == '$' && !quotes[0] && !is_space(input[i[0] + 1]))
	{
		i[0]++;
		size += env_var_size(input, i);
	}
	else if (input[i[0]] == '*' && !quotes[0] && !quotes[1] && i[1] == 0)
		size += wildcard_size(input, i) - 1;
	else
	{
		size++;
		i[0]++;
	}
	return (size);
}

int	get_new_size(char *input, int exit_code, int flag)
{
	int		i[2];
	int		size;
	bool	quotes[2];

	i[0] = 0;
	i[1] = flag;
	size = 0;
	quotes[0] = false;
	quotes[1] = false;
	while (input[i[0]])
	{
		size += calculate_size(input, i, exit_code, quotes);
	}
	return (size);
}
