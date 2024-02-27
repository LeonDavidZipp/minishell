/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:07:56 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/27 10:52:52 by intra            ###   ########.fr       */
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
	bool	was_flag;

	i = 0;
	amount = 0;
	flags[0] = false;
	flags[1] = false;
	flags[2] = false;
	was_flag = false;
	while (input[i])
	{
		quotes_brackets(input[i], &flags[0], &flags[1], &flags[2]);
		if ((!flags[0] && !flags[1] && !flags[2] && !is_space(input[i]))
			&& (i == 0 || (is_space(input[i - 1]) || input[i - 1] == '\''
					|| input[i - 1] == '"' || input[i - 1] == '(' || was_flag)))
		{
			amount++;
		}
		was_flag = flags[0] || flags[1] || flags[2];
		i++;
	}
	return (amount);
}

int	condition(char *input, int *count, bool *flags, bool was_flag)
{
	if ((!flags[0] && !flags[1] && !flags[2] && !is_space(input[count[1]]))
		&& (count[1] == 0 || (is_space(input[count[1] - 1])
				|| input[count[1] - 1] == '\'' || input[count[1] - 1] == '"'
				|| input[count[1] - 1] == '(' || was_flag
				|| (ft_isprint(input[count[1] - 1]) && (input[count[1]] == '\''
						|| input[count[1]] == '"' || input[count[1]] == '(')))))
	{
		return (1);
	}
	return (0);
}

int	process_token(char *input, int *count, bool *flags, char **tokens)
{
	bool	was_flag;

	was_flag = false;
	while (input[count[0]] && is_space(input[count[0]])
		&& !flags[0] && !flags[1] && !flags[2])
		count[0]++;
	count[1] = count[0];
	while (input[count[1]])
	{
		quotes_brackets(input[count[1]], &flags[0], &flags[1], &flags[2]);
		count[1]++;
		was_flag = flags[0] || flags[1] || flags[2];
		if (condition(input, count, flags, was_flag))
			break ;
	}
	if (count[1] > 0 && input[count[1] - 1] == ' ')
		count[1]--;
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

// int main()
// {
// 	char **str = split("'\"'\"'\"hello\"'\"'\"' test hallo echo hi&&echo hi");
// 	// char **str = split("echo hi");
// 	for (int i = 0; str[i]; i++)
// 	{
// 		printf("%s\n", str[i]);
// 	}
// 	for (int i = 0; str[i]; i++)
// 	{
// 		free(str[i]);
// 	}
// 	free(str);
// 	return 0;
// }