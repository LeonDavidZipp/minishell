/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:49:48 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/20 12:02:05 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_quotes_brackets(char c, bool *in_quote, bool *in_bracket)
{
	if (c == '\'' || c == '\"')
		*in_quote = !*in_quote;
	if (c == '(')
		*in_bracket = true;
	if (c == ')')
		*in_bracket = false;
}

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
	char	*new_input;
	int		i;
	int		j;
	bool	in_quote;
	bool	in_bracket;

	i = 0;
	j = 0;
	in_quote = false;
	in_bracket = false;
	while (input[i])
	{
		handle_quotes_brackets(input[i], &in_quote, &in_bracket);
		if (input[i] && input[i + 1] && is_operator(input[i], input[i + 1])
			&& !in_quote && !in_bracket)
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
	bool	in_quote;
	bool	in_bracket;

	i = 0;
	j = 0;
	in_quote = false;
	in_bracket = false;
	new_input = malloc(sizeof(char) * ((ft_strlen(input)
					+ new_input_length(input)) + 1));
	if (!new_input)
		return (NULL);
	while (input[i])
	{
		handle_quotes_brackets(input[i], &in_quote, &in_bracket);
		if (input[i] && input[i + 1] && is_operator(input[i], input[i + 1])
			&& !in_quote && !in_bracket)
			check_if_space_needed(input, new_input, &j, &i);
		else
			new_input[j++] = input[i++];
	}
	new_input[j] = '\0';
	return (new_input);
}
