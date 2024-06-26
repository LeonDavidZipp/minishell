/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:52:06 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/18 11:54:43 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	process_value(t_expand **data, char **value, char **name);

int	exit_code_expand(char **str, int *j, int last_exit_code)
{
	char	*exit_code;
	int		i;

	exit_code = ft_itoa(last_exit_code);
	if (!exit_code)
		return (0);
	i = 0;
	while (exit_code[i])
	{
		(*str)[*j] = exit_code[i];
		(*j)++;
		i++;
	}
	free(exit_code);
	return (1);
}

int	var_expand(t_expand *data)
{
	char	*name;
	char	*value;
	char	*input;
	int		j;

	input = data->input + data->i[0];
	j = 1;
	if (input[j - 1] == '~')
		name = ft_strdup("HOME");
	else
	{
		while (ft_isalnum(input[j]) || input[j] == '_')
			j++;
		name = ft_substr(input, 1, j - 1);
		if (!name)
			return (0);
	}
	if (input[j - 1] == '~')
		value = getenv(name);
	else
		value = ft_getenv(name, data->env_vars);
	process_value(&data, &value, &name);
	return (1);
}

static void	process_value(t_expand **data, char **value_ref, char **name)
{
	int		j;
	char	*value;

	value = *value_ref;
	if (value)
	{
		j = 0;
		while (value[j])
		{
			if (!(*data)->quotes[1] && (*data)->i[1] == 0 && j == 0
				&& value[j] == ' ')
				j++;
			if (j > 0 && value[j] == ' ' && value[j - 1] == ' '
				&& !(*data)->quotes[1])
			{
				j++;
				continue ;
			}
			(*(*data)->output)[((*data)->i[1])++] = value[j++];
		}
		free(*name);
	}
	else
		free(*name);
}

int	handle_dollar(t_expand *data)
{
	if (data->input[data->i[0]] == '$' && data->input[data->i[0] + 1] == '?')
	{
		if (!exit_code_expand(data->output, &data->i[1], data->exit_code))
			return (free(*data->output), 0);
		data->i[0] += 2;
	}
	else if (data->input[data->i[0]] == '$' || data->input[data->i[0]] == '~')
	{
		if (!var_expand(data))
			return (free(*data->output), 0);
		data->i[2] = 1;
		while (ft_isalnum(data->input[data->i[0] + data->i[2]])
			|| data->input[data->i[0] + data->i[2]] == '_')
			data->i[2]++;
		data->i[0] += data->i[2];
	}
	return (1);
}
