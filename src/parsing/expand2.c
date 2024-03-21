/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:52:06 by cgerling          #+#    #+#             */
/*   Updated: 2024/03/21 17:33:48 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

int	var_expand(char *input, char **output, int *j)
{
	char	*name;
	char	*value;
	int		i;

	i = 1;
	while (ft_isalnum(input[i]) || input[i] == '_')
		i++;
	name = ft_substr(input, 1, i - 1);
	if (!name)
		return (0);
	value = getenv(name);
	if (value)
	{
		i = 0;
		while (value[i])
			(*output)[(*j)++] = value[i++];
		free(name);
	}
	else
		free(name);
	return (1);
}

int	handle_dollar(char *input, char **output, int *i)
{
	if (input[i[0]] == '$' && input[i[0] + 1] == '?')
	{
		if (!exit_code_expand(output, &i[1], i[4]))
			return (free(*output), 0);
		i[0] += 2;
	}
	else if (input[i[0]] == '$')
	{
		if (!var_expand(input + i[0], output, &i[1]))
			return (free(*output), 0);
		i[2] = 1;
		while (ft_isalnum(input[i[0] + i[2]]) || input[i[0] + i[2]] == '_')
			i[2]++;
		i[0] += i[2];
	}
	return (1);
}
