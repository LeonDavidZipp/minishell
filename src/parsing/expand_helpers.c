/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:05:35 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/12 13:54:14 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	calculate_size(char *input, int *i, char **env_vars, bool *quotes);
int	env_var_size(char *input, int *i, char **env_vars);
int	wildcard_size(char *input, int *i);
int	calc_wildcard_size(DIR *dir, char *pattern, int position);

char	*ft_getenv(char *name, char **env_vars)
{
	int		i;
	int		j;
	char	*value;

	i = 0;
	while (env_vars[i])
	{
		j = 0;
		while (env_vars[i][j] == name[j] && name[j])
			j++;
		if (env_vars[i][j] == '=' && !name[j])
		{
			value = ft_strdup(env_vars[i] + j + 1);
			if (!value)
				return (NULL);
			return (value);
		}
		i++;
	}
	return (NULL);
}

int	get_new_size(char *input, int exit_code, char **env_vars, int *flags)
{
	int		i[4];
	int		size;
	bool	quotes[2];

	i[0] = 0;
	i[1] = flags[0];
	i[2] = flags[1];
	i[3] = exit_code;
	size = 0;
	quotes[0] = false;
	quotes[1] = false;
	while (input[i[0]])
	{
		size += calculate_size(input, i, env_vars, quotes);
	}
	return (size);
}

// ein dollar ohne quotes schreibt nur das dollar sign wenn nichts mehr dahinter steht ansonsten versucht es immer zu expanden???

int	is_valid_dollar(char *input, int i, bool *quotes) // need to modify this, doesn't work correctly yet
{
	(void)quotes;
	if (input[i] != '\0' && (input[i + 1] == '?' || ft_isalnum(input[i + 1]) || input[i + 1] == '_'))
		return (1);
	if (input[i] != '\0' && (input[i + 1] == '\"' || input[i + 1] == '\''))
	{
		while(input[i])
		{
			if (ft_isalnum(input[i]) || input[i] == '_')
				return (1);
			i++;
		}
	}
	return (0);
}

int	calculate_size(char *input, int *i, char **env_vars, bool *quotes)
{
	int		size;

	size = 0;
	handle_quotes(input[i[0]], &quotes[0], &quotes[1]);
	if (input[i[0]] == '$' && input[i[0] + 1] == '?' && !quotes[0] && i[2] == 0)
	{
		size += ft_dec_len(i[3]);
		i[0] += 2;
	}
	else if ((input[i[0]] == '$' && !quotes[0] && is_valid_dollar(input, i[0], quotes) && i[2] == 0)
		|| (input[i[0]] == '~' && !quotes[0] && !quotes[1] && (is_space(input[i[0] + 1]) || input[i[0] + 1] == '/') && i[2] == 0))
	{
		i[0]++;
		size += env_var_size(input, i, env_vars);
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

int	env_var_size(char *input, int *i, char **env_vars)
{
	int		start;
	int		size;
	char	*name;
	char	*value;

	start = i[0];
	if (input[i[0] - 1] == '~')
		name = ft_strdup("HOME"); // NULL check ?! but then too many lines
	else
	{
		while (ft_isalnum(input[i[0]]) || input[i[0]] == '_')
			i[0]++;
		name = ft_substr(input, start, i[0] - start);
		if (!name)
			return (0);
	}
	value = ft_getenv(name, env_vars);
	if (value)
		size = ft_strlen(value);
	else
		size = 0;
	free(name);
	return (size);
}

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

int	empty_entry(char *entry) // maybe rename because it doesn't check only complete empty entries
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
