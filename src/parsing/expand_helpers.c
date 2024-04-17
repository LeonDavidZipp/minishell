/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:05:35 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/17 16:27:26 by lzipp            ###   ########.fr       */
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

	if (!name)
		return (NULL);
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

int	is_valid_dollar(char *input, int i, bool *quotes)
{
	if (input[i] != '\0' && !quotes[1] && input[i + 1] != '\0'
		&& input[i + 1] != '/')
		return (1);
	if (input[i] != '\0' && (input[i + 1] == '?'
			|| ft_isalnum(input[i + 1]) || input[i + 1] == '_'))
		return (1);
	if (input[i] != '\0' && (input[i + 1] == '\"' || input[i + 1] == '\''))
	{
		while (input[i])
		{
			if (ft_isalnum(input[i]) || input[i] == '_')
				return (1);
			i++;
		}
	}
	return (0);
}

int	calculate_size(char *inp, int *i, char **env_vars, bool *quotes)
{
	int		size;

	size = 0;
	handle_quotes(inp[i[0]], &quotes[0], &quotes[1]);
	if (inp[i[0]] == '$' && inp[i[0] + 1] == '?' && !quotes[0] && i[2] == 0)
	{
		size += ft_dec_len(i[3]);
		i[0] += 2;
	}
	else if ((inp[i[0]] == '$' && !quotes[0]
			&& is_valid_dollar(inp, i[0], quotes) && i[2] == 0)
		|| (inp[i[0]] == '~' && !quotes[0] && !quotes[1]
			&& (is_space(inp[i[0] + 1]) || inp[i[0] + 1] == '/') && i[2] == 0))
	{
		i[0]++;
		size += env_var_size(inp, i, env_vars);
	}
	else if (inp[i[0]] == '*' && !quotes[0] && !quotes[1] && i[1] == 0)
		size += wildcard_size(inp, i) - 1;
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
		name = ft_strdup("HOME");
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
	free(value);
	return (size);
}
