/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 14:47:38 by cgerling          #+#    #+#             */
/*   Updated: 2024/03/26 11:05:43 by lzipp            ###   ########.fr       */
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
	if (s_quote % 2 != 0 || d_quote % 2 != 0 || bracket_1 != bracket_2)
		return (ft_fprintf(2, "%s: invalid input: "
				"quotes or brackets not closed\n", NAME), 1);
	return (0);
}

// int	main(void)
// {
// 	char *input = "echo 'hel\"'lo' world";
// 	if (check_input(input) == 0)
// 		printf("Input is valid\n");
// 	return (0);
// }
