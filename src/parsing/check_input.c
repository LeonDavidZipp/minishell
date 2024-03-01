/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 14:47:38 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/29 15:37:42 by cgerling         ###   ########.fr       */
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
	flags[2] = false;
}

int	check_input(char *str)
{
	int		s_quote;
	int		d_quote;
	int		bracket_1;
	int		bracket_2;
	bool	flags[3];

	init_flags(flags);
	init_counts(&s_quote, &d_quote, &bracket_1, &bracket_2);
	while (*str)
	{
		quotes_brackets(*str, &flags[0], &flags[1], &flags[2]);
		if (*str == '\'' && !flags[1] && !flags[2])
			s_quote++;
		if (*str == '\"' && !flags[0] && !flags[2])
			d_quote++;
		if (*str == '(' && !flags[0] && !flags[1])
			bracket_1++;
		if (*str == ')' && !flags[0] && !flags[1])
			bracket_2++;
		str++;
	}
	if (s_quote % 2 != 0 || d_quote % 2 != 0 || bracket_1 != bracket_2)
		return (printf("Error: invalid input - "
				"quotes or brackets not closed\n"), 1);
	return (0);
}

// int	main(void)
// {
// 	char *input = "echo 'hel\"'lo' world";
// 	if (check_input(input) == 0)
// 		printf("Input is valid\n");
// 	return (0);
// }
