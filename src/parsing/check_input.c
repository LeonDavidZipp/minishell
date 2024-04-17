/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 14:47:38 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/16 11:54:53 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	init_counts(int *s_quote, int *d_quote, int *brack_1, int *brack_2)
{
	*s_quote = 0;
	*d_quote = 0;
	*brack_1 = 0;
	*brack_2 = 0;
}

static void	init_flags(bool *flags)
{
	flags[0] = false;
	flags[1] = false;
}

static int	return_error(int bracket_1, int bracket_2, int s_quote, int d_quote)
{
	if (bracket_2 > bracket_1)
	{
		ft_fprintf(2, "%s: syntax error near unexpected token ')'\n", NAME);
		return (2);
	}
	if (s_quote % 2 != 0 || d_quote % 2 != 0 || bracket_1 != bracket_2)
	{
		ft_fprintf(2, "%s: invalid input: quotes or brackets not closed\n",
			NAME);
		return (2);
	}
	return (0);
}

int	check_input(char *str)
{
	int		s_quote;
	int		d_quote;
	int		bracket_1;
	int		bracket_2;
	bool	flags[2];

	init_flags(flags);
	init_counts(&s_quote, &d_quote, &bracket_1, &bracket_2);
	while (*str)
	{
		handle_quotes(*str, &flags[0], &flags[1]);
		if (*str == '\'' && !flags[1])
			s_quote++;
		if (*str == '\"' && !flags[0])
			d_quote++;
		if (*str == '(' && !flags[0] && !flags[1])
			bracket_1++;
		if (*str == ')' && !flags[0] && !flags[1])
			bracket_2++;
		str++;
	}
	return (return_error(bracket_1, bracket_2, s_quote, d_quote));
}

// int	main(void)
// {
// 	char *input = "echo 'hel\"'lo' world";
// 	if (check_input(input) == 0)
// 		printf("Input is valid\n");
// 	return (0);
// }
