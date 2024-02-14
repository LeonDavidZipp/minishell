/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 10:43:26 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/14 11:02:01 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// expanding should happen before parsing

char	*expand(char *input, char **envp)
{
	char	*output;
	bool	s_quotes;
	bool	d_quotes;
	int		i;
	int		j;

	i = 0;
	j = 0;
	s_quotes = false;
	d_quotes = false;
	output = malloc(sizeof(char) * get_size(input) + 1); // implement get_size
	if (!output)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\'')
			s_quotes = !s_quotes;
		if (input[i] == '\"')
			d_quotes = !d_quotes;
		if (input[i] == '$' && input[i + 1] == '?' && !s_quotes && !d_quotes)
		{
			// expand $?
		}
		else if (input[i] == '$' && !s_quotes && !d_quotes)
		{
			// expand $var
		}
		else if (input[i] == '*' && !s_quotes && !d_quotes)
		{
			// expand *
		}
		else
		{
			output[j++] = input[i++];
		}
	}
	output[j] = '\0';
	return (output);
}
