/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_helpers2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/02/23 20:50:53 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../inc/minishell.h"

void	quotes_brackets(char c, bool *s_quote, bool *d_quote, bool *in_bracket)
{
	if (c == '\'' && !*d_quote && !*in_bracket)
		*s_quote = !*s_quote;
	if (c == '"' && !*s_quote && !*in_bracket)
		*d_quote = !*d_quote;
	if (c == '(' && !*s_quote && !*d_quote)
		*in_bracket = true;
	if (c == ')' && !*s_quote && !*d_quote)
		*in_bracket = false;
}

int	is_operator(char c, char d)
{
	if (c == d && (c == '|' || c == '&' || c == '<' || c == '>'))
		return (2);
	else if ((c == '|' || c == '<' || c == '>') && d != c)
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\0')
		return (1);
	return (0);
}