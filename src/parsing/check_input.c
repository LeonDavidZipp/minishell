/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 14:47:38 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/19 15:30:24 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

static void	init_vars(int *s_quote, int *d_quote, int *bracket_1, int *bracket_2)
{
	*s_quote = 0;
	*d_quote = 0;
	*bracket_1 = 0;
	*bracket_2 = 0;
}

int	check_input(char *str)
{
	int	s_quote;
	int	d_quote;
	int	bracket_1;
	int	bracket_2;
	int	i;

	i = 0;
	init_vars(&s_quote, &d_quote, &bracket_1, &bracket_2);
	while (str[i])
	{
		if (str[i] == '\'')
			s_quote++;
		if (str[i] == '\"')
			d_quote++;
		if (str[i] == '(')
			bracket_1++;
		if (str[i] == ')')
			bracket_2++;
		i++;
	}
	if (s_quote % 2 != 0 || d_quote % 2 != 0 || bracket_1 != bracket_2)
		return (printf("Error: invalid input - quotes or brackets not closed\n"), 1);
	return (0);
}

int	main(int argc, char **argv)
{
	char *input = "echo hello world";
	if (check_input(input) == 0)
		printf("Input is valid\n");
	return (0);
}
