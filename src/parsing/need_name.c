/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   need_name.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:52:00 by cgerling          #+#    #+#             */
/*   Updated: 2024/03/22 15:19:24 by cgerling         ###   ########.fr       */
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

char	*remove_quotes(char *token)
{
	char	*output;
	bool	flags[2];
	int		i[2];

	init(i, flags);
	output = malloc(ft_strlen(token) + 1);
	if (!output)
		return (NULL);
	while (token[i[0]])
	{
		handle_quotes(token[i[0]], &flags[0], &flags[1]);
		if (token[i[0]] == '\'' && !flags[1])
		{
			i[0]++;
			continue ;
		}
		if (token[i[0]] == '\"' && !flags[0])
		{
			i[0]++;
			continue ;
		}
		output[i[1]++] = token[i[0]++];
	}
	output[i[1]] = '\0';
	return (output);
}

char	*quote_remove_in_place(char *token)
{
	char	*result;

	result = remove_quotes(token);
	if (!result)
		return (NULL);
	free(token);
	return (result);
}

char	**fill_new_tokens(char **tokens, char **new_tokens, int exit_code)
{
	char	*temp;
	char	*expanded;
	int		is_heredoc;
	int		i;

	i = 0;
	is_heredoc = 0;
	while (tokens[i])
	{
		expanded = expand(tokens[i], exit_code, 0);
		if (!expanded)
			return (ft_free_2d_arr((void **)tokens), \
			ft_free_2d_arr((void **)new_tokens), NULL);
		if (i > 0 && ft_strcmp(tokens[i - 1], "<<") == 0)
			is_heredoc = 1;
		if (is_heredoc == 1)
		{
			new_tokens[i] = expanded;
			is_heredoc = 0;
		}
		else
		{
			temp = remove_quotes(expanded);
			if (!temp)
				return (ft_free_2d_arr((void **)tokens), \
				ft_free_2d_arr((void **)new_tokens), free(expanded), NULL);
			new_tokens[i] = temp;
			free(expanded);
		}
		i++;
	}
	ft_free_2d_arr((void **)tokens);
	return (new_tokens);
}

char	**expand_and_remove(char **tokens, int exit_code)
{
	char	**new_tokens;
	int		i;

	i = 0;
	while (tokens[i])
		i++;
	new_tokens = malloc(sizeof(char *) * (i + 1));
	if (!new_tokens)
		return (ft_free_2d_arr((void **)tokens), NULL);
	new_tokens[i] = NULL;
	return (fill_new_tokens(tokens, new_tokens, exit_code));
}

// int main()
// {
// 	char *token = "'\"'\"'\"hello\"'\"'\"'";
// 	printf("%s\n", remove_quotes(token));
// 	return 0;
// }