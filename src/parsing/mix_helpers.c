/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mix_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 21:41:13 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/06 16:21:10 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_operator(char c, char d)
{
	if (c == d && (c == '|' || c == '&' || c == '<' || c == '>'))
		return (2);
	else if (((c == '|' && d != '|') || (c == '<' && d != '<')
		|| (c == '>' && d != '>') || c == '(' || c == ')'))
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\0')
		return (1);
	return (0);
}

void	handle_quotes(char c, bool *s_quote, bool *d_quote)
{
	if (c == '\'' && !*d_quote)
		*s_quote = !*s_quote;
	else if (c == '"' && !*s_quote)
		*d_quote = !*d_quote;
}

char	*get_pattern(char *input, int *i, int *position, int *start)
{
	// int		start;
	int		end;
	int		tmp;
	char	*pattern;

	tmp = *i;
	// start = 0;
	end = 0;
	while (*i >= 0 && !is_space(input[*i]))
		(*i)--;
	*start = *i + 1;
	*position = tmp - *start;
	*i = tmp;
	while (input[*i] && !is_space(input[*i]))
		(*i)++;
	end = *i;
	pattern = ft_substr(input, *start, end - *start);
	if (!pattern)
		return (NULL);
	return (pattern);
}

bool	match(char *pattern, char *string)
{
	while (*pattern && *string)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			while (*string)
			{
				if (match(pattern, string))
					return (true);
				string++;
			}
			return (match(pattern, string));
		}
		else if (*pattern == *string)
		{
			pattern++;
			string++;
		}
		else
			return (false);
	}
	return (*pattern == *string);
}
