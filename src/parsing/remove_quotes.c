/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:52:00 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/29 15:53:17 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	init(int *i, bool *flags)
{
	i[0] = 0;
	i[1] = 0;
	flags[0] = false;
	flags[1] = false;
	flags[2] = false;
}

char	*remove_quotes(char *token)
{
	char	*output;
	bool	flags[3];
	int		i[2];

	init(i, flags);
	output = malloc(ft_strlen(token) + 1);
	if (!output)
		return (NULL);
	while (token[i[0]])
	{
		quotes_brackets(token[i[0]], &flags[0], &flags[1], &flags[2]);
		if (token[i[0]] == '\'' && !flags[1] && !flags[2])
		{
			i[0]++;
			continue ;
		}
		if (token[i[0]] == '\"' && !flags[0] && !flags[2])
		{
			i[0]++;
			continue ;
		}
		output[i[1]++] = token[i[0]++];
	}
	output[i[1]] = '\0';
	return (output);
}

// int main()
// {
// 	char *token = "'\"'\"'\"hello\"'\"'\"'";
// 	printf("%s\n", remove_quotes(token));
// 	return 0;
// }