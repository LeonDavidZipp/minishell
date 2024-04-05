/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 20:50:28 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/04 11:15:16 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	count_if_space_needed(char *input, int *j, int *i)
{
	if (is_operator(input[*i], input[*i + 1]) == 2)
	{
		if (*i > 0 && !is_space(input[*i - 1]))
			(*j)++;
		if (input[*i + 2] && !is_space(input[*i + 2]))
			(*j)++;
	}
	else
	{
		if (*i > 0 && !is_space(input[*i - 1]))
			(*j)++;
		if (input[*i + 1] && !is_space(input[*i + 1]))
			(*j)++;
	}
}

int	new_input_length(char *input)
{
	int		i;
	int		j;
	bool	s_quote;
	bool	d_quote;

	i = 0;
	j = 0;
	s_quote = false;
	d_quote = false;
	while (input[i])
	{
		handle_quotes(input[i], &s_quote, &d_quote);
		if (input[i] && (input[i + 1] || input[i + 1] == '\0')
			&& is_operator(input[i], input[i + 1])
			&& !s_quote && !d_quote)
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
		if (*i > 0 && !is_space(input[*i - 1]))
			new_input[(*j)++] = ' ';
		new_input[(*j)++] = input[(*i)++];
		new_input[(*j)++] = input[(*i)++];
		if (!is_space(input[*i]))
			new_input[(*j)++] = ' ';
	}
	else
	{
		if (*i > 0 && !is_space(input[*i - 1]))
			new_input[(*j)++] = ' ';
		new_input[(*j)++] = input[(*i)++];
		if (!is_space(input[*i]))
			new_input[(*j)++] = ' ';
	}
}

char	*add_spaces(char *input)
{
	char	*new_input;
	int		i;
	int		j;
	bool	flags[2];

	i = 0;
	j = 0;
	flags[0] = false;
	flags[1] = false;
	new_input = malloc(sizeof(char) * ((ft_strlen(input)
					+ new_input_length(input)) + 1));
	if (!new_input)
		return (ft_fprintf(2, "%s: malloc: %s\n", NAME, strerror(errno)), NULL);
	while (input[i])
	{
		handle_quotes(input[i], &flags[0], &flags[1]);
		if (input[i] && (input[i + 1] || input[i + 1] == '\0')
			&& is_operator(input[i], input[i + 1])
			&& !flags[0] && !flags[1])
			check_if_space_needed(input, new_input, &j, &i);
		else
			new_input[j++] = input[i++];
	}
	new_input[j] = '\0';
	return (new_input);
}
