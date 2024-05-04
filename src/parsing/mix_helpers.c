/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mix_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 21:41:13 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/16 15:34:57 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	handle_wildcard2(char **pattern, char **string,
				bool *s_quote, bool *d_quote);

void	handle_quotes(char c, bool *s_quote, bool *d_quote)
{
	if (c == '\'' && !*d_quote)
		*s_quote = !*s_quote;
	else if (c == '"' && !*s_quote)
		*d_quote = !*d_quote;
}

char	*get_pattern(char *input, int *i, int *position, int *start)
{
	int		end;
	int		tmp;
	char	*pattern;
	bool	s_quote;
	bool	d_quote;

	tmp = *i;
	s_quote = false;
	d_quote = false;
	while (*i >= 0 && (!is_space(input[*i]) || s_quote || d_quote))
	{
		handle_quotes(input[*i], &s_quote, &d_quote);
		(*i)--;
	}
	*start = *i + 1;
	*position = tmp - *start;
	*i = tmp;
	while (input[*i] && (!is_space(input[*i]) || s_quote || d_quote))
	{
		handle_quotes(input[*i], &s_quote, &d_quote);
		(*i)++;
	}
	end = *i;
	pattern = ft_substr(input, *start, end - *start);
	return (pattern);
}

bool	match(char *pattern, char *string, bool s_quote, bool d_quote)
{
	while (*pattern && *string)
	{
		handle_quotes(*pattern, &s_quote, &d_quote);
		if ((*pattern == '\'' && !d_quote) || (*pattern == '"' && !s_quote))
		{
			pattern++;
			continue ;
		}
		if (!s_quote && !d_quote && *pattern == '*')
			return (handle_wildcard2(&pattern, &string, &s_quote, &d_quote));
		else if (*pattern == *string)
		{
			pattern++;
			string++;
		}
		else
			return (false);
	}
	handle_quotes(*pattern, &s_quote, &d_quote);
	while (*pattern == '\'' || *pattern == '"' || (*pattern == '*'
			&& !s_quote && !d_quote))
		pattern++;
	return (*pattern == *string);
}

static bool	handle_wildcard2(char **pattern, char **string,
				bool *s_quote, bool *d_quote)
{
	while (**pattern == '*')
		(*pattern)++;
	while (**string)
	{
		if (match(*pattern, *string, *s_quote, *d_quote))
			return (true);
		(*string)++;
	}
	return (match(*pattern, *string, *s_quote, *d_quote));
}
