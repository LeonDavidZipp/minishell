/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:02:42 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/16 14:19:58 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	wildcard_size(char *input, int *i)
{
	char			*pattern;
	int				position;
	int				size;
	int				start;
	DIR				*dir;

	size = 0;
	start = 0;
	pattern = get_pattern(input, i, &position, &start);
	if (!pattern)
		return (0);
	dir = opendir(".");
	if (dir == NULL)
		return (0);
	size = calc_wildcard_size(dir, pattern, position);
	if (size == 0)
		size = ft_strlen(pattern) + 1;
	closedir(dir);
	free(pattern);
	return (size);
}

int	empty_entry(char *entry)
{
	int	len;

	len = ft_strlen(entry);
	if (entry[0] == ' ' && entry[len - 1] == ' ')
		return (3);
	else if (entry[0] == ' ')
		return (1);
	else if (entry[len - 1] == ' ')
		return (2);
	return (0);
}

int	calc_wildcard_size(DIR *dir, char *pattern, int position)
{
	struct dirent	*entry;
	int				size;
	bool			flag;
	bool			s_quote;
	bool			d_quote;
	int				i;

	flag = false;
	s_quote = false;
	d_quote = false;
	size = 0;
	i = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		while (pattern[i] == '\'' || pattern[i] == '"')
			i++;
		if (pattern[i] == '.' || (entry->d_name[0] != '.' || (entry->d_name[0] != '.' && entry->d_name[1] != '.')))
		{
			if (match(pattern, entry->d_name, s_quote, d_quote))
			{
				if (!flag)
				{
					size -= position;
					flag = true;
				}
				size += ft_strlen(entry->d_name) + 1;
				if (empty_entry(entry->d_name) == 3)
					size += 2;
				else if (empty_entry(entry->d_name) == 1 || empty_entry(entry->d_name) == 2)
					size++;
			}
		}
		entry = readdir(dir);
	}
	return (size);
}
