/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:49:48 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/23 17:26:20 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	count_if_space_needed(char *input, int *j, int *i)
{
	if (is_operator(input[*i], input[*i + 1]) == 2)
	{
		if (input[*i - 1] && !ft_isspace(input[*i - 1]))
			(*j)++;
		if (input[*i + 2] && !ft_isspace(input[*i + 2]))
			(*j)++;
	}
	else
	{
		if (input[*i - 1] && !ft_isspace(input[*i - 1]))
			(*j)++;
		if (input[*i + 1] && !ft_isspace(input[*i + 1]))
			(*j)++;
	}
}

int	new_input_length(char *input)
{
	int		i;
	int		j;
	bool	s_quote;
	bool	d_quote;
	bool	in_bracket;

	i = 0;
	j = 0;
	s_quote = false;
	d_quote = false;
	in_bracket = false;
	while (input[i])
	{
		quotes_brackets(input[i], &s_quote, &d_quote, &in_bracket);
		if (input[i] && input[i + 1] && is_operator(input[i], input[i + 1])
			&& !s_quote && !d_quote && !in_bracket)
		{
			count_if_space_needed(input, &j, &i);
		}
		i++;
	}
	return (j);
}

void	check_if_space_needed(char *input, char *new_input, int *j, int *i)
{
	if (is_operator(input[*i], input[*i + 1]) == 2)
	{
		if (input[*i - 1] && !ft_isspace(input[*i - 1]))
			new_input[(*j)++] = ' ';
		new_input[(*j)++] = input[(*i)++];
		new_input[(*j)++] = input[(*i)++];
		if (!ft_isspace(input[*i]))
			new_input[(*j)++] = ' ';
	}
	else
	{
		if (input[*i - 1] && !ft_isspace(input[*i - 1]))
			new_input[(*j)++] = ' ';
		new_input[(*j)++] = input[(*i)++];
		if (!ft_isspace(input[*i]))
			new_input[(*j)++] = ' ';
	}
}

char	*add_spaces(char *input)
{
	char	*new_input;
	int		i;
	int		j;
	bool	flags[3];

	i = 0;
	j = 0;
	flags[0] = false;
	flags[1] = false;
	flags[2] = false;
	new_input = malloc(sizeof(char) * ((ft_strlen(input)
					+ new_input_length(input)) + 1));
	if (!new_input)
		return (NULL);
	while (input[i])
	{
		quotes_brackets(input[i], &flags[0], &flags[1], &flags[2]);
		if (input[i] && input[i + 1] && is_operator(input[i], input[i + 1])
			&& !flags[0] && !flags[1] && !flags[2])
			check_if_space_needed(input, new_input, &j, &i);
		else
			new_input[j++] = input[i++];
	}
	new_input[j] = '\0';
	return (new_input);
}
