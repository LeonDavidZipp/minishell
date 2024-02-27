/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:52:00 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/27 16:49:39 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*remove_quotes(char *token)
{
	char	*output;
	bool	flags[3];
	int		i;
	int		j;

	i = 0;
	j = 0;
	flags[0] = false;
	flags[1] = false;
	flags[2] = false;
	output = malloc(ft_strlen(token) + 1);
	if (!output)
		return (NULL);
	while (token[i])
	{
		quotes_brackets(token[i], &flags[0], &flags[1], &flags[2]);
		if (token[i] == '\'' && !flags[1] && !flags[2])
		{
			i++;
			continue ;
		}
		if (token[i] == '\"' && !flags[0] && !flags[2])
		{
			i++;
			continue ;
		}
		output[j] = token[i];
		i++;
		j++;
	}
	output[j] = '\0';
	return (output);
}

// int main()
// {
// 	char *token = "'\"'\"'\"hello\"'\"'\"'";
// 	printf("%s\n", remove_quotes(token));
// 	return 0;
// }