/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:07:56 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/27 18:24:04 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	init_vars(int *count, bool *flags)
{
	count[0] = 0;
	count[1] = 0;
	count[2] = 0;
	flags[0] = false;
	flags[1] = false;
	flags[2] = false;
}

int	count_tokens(char *input)
{
	int		i;
	int		amount;
	bool	flags[3];

	i = 0;
	amount = 0;
	flags[0] = false;
	flags[1] = false;
	flags[2] = false;
	while (input[i])
	{
		while (input[i] && is_space(input[i]) && !flags[0] && !flags[1] && !flags[2])
			i++;
		if (input[i])
			amount++;
		while (input[i] && (!is_space(input[i]) || flags[0] || flags[1] || flags[2]))
		{
			quotes_brackets(input[i], &flags[0], &flags[1], &flags[2]);
			i++;
		}
	}
	return (amount);
}

int	process_token(char *input, int *count, bool *flags, char **tokens)
{
	while (input[count[0]] && is_space(input[count[0]])
		&& !flags[0] && !flags[1] && !flags[2])
		count[0]++;
	count[1] = count[0];
	while ((input[count[1]] && (!is_space(input[count[1]])
				|| flags[0] || flags[1] || flags[2])))
	{
		quotes_brackets(input[count[1]], &flags[0], &flags[1] , &flags[2]);
		count[1]++;
	}
	tokens[count[2]] = ft_substr(input, count[0], count[1] - count[0]);
	if (!tokens[count[2]])
		return (ft_free_2d_arr((void **)tokens), free(input), 0);
	count[0] = count[1];
	count[2]++;
	return (1);
}

char	**split(char *input)
{
	char	**tokens;
	char	*new_input;
	int		count[4];
	bool	flags[3];

	init_vars(count, flags);
	new_input = add_spaces(input);
	if (!new_input)
		return (NULL);
	count[3] = count_tokens(new_input);
	tokens = malloc(sizeof(char *) * (count[3] + 1));
	if (!tokens)
		return (free(new_input), NULL);
	tokens[count[3]] = NULL;
	while (count[2] < count[3])
	{
		if (!process_token(new_input, count, flags, tokens))
			return (free(new_input), free(tokens), NULL);
	}
	free(new_input);
	return (tokens);
}

char	*remove_expand(char *token)
{
	char	*output;
	char	*temp;

	if 
}

int main()
{
	char *input = "'\"'\"'\"hello\"'\"'\"' test hallo echo hi&&echo hi'test' 'hallo'";
	if (check_input(input) == 1)
		return (printf("Error: invalid input - quotes or brackets not closed\n"), 1);
	char **str = split(input);
	if (!str)
		return 1;
	char *temp;
	for (int i = 0; str[i]; i++)
	{
		temp = remove_expand(str[i]);
		if (!temp)
			return 1;
		printf("%s\n", temp);
		free(temp);
	}
	for (int i = 0; str[i]; i++)
	{
		free(str[i]);
	}
	free(str);
	return 0;
}