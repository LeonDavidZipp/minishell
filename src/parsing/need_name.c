/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   need_name.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:52:00 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/09 14:47:44 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	init(int *i, bool *flags)
{
	i[0] = 0;
	i[1] = 0;
	flags[0] = false;
	flags[1] = false;
}

char	*remove_quotes(char *str)
{
	char	*output;
	bool	flags[2];
	int		i[2];

	init(i, flags);
	output = malloc(ft_strlen(str) + 1);
	if (!output)
		return (NULL);
	while (str[i[0]])
	{
		handle_quotes(str[i[0]], &flags[0], &flags[1]);
		if (str[i[0]] == '\'' && !flags[1])
		{
			i[0]++;
			continue ;
		}
		if (str[i[0]] == '\"' && !flags[0])
		{
			i[0]++;
			continue ;
		}
		output[i[1]++] = str[i[0]++];
	}
	output[i[1]] = '\0';
	return (output);
}

char	*expand_and_remove(char *str, int exit_code, char **env_vars, int flag)
{
	char	*expanded;
	char	*result;
	int		flags[2];

	if (!str)
		return (NULL);
	flags[0] = 0;
	flags[1] = flag;
	expanded = expand(str, exit_code, env_vars, flags);
	if (!expanded)
		return (NULL);
	result = remove_quotes(expanded);
	if (!result)
		return (free(expanded), NULL);
	free(expanded);
	return (result);
}
