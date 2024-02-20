/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:07:56 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/20 12:20:04 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	init_vars(int *count, bool *flags)
{
	count[0] = 0;
	count[1] = 0;
	count[2] = 0;
	flags[0] = false;
	flags[1] = false;
}

int	is_operator(char c, char d)
{
	if (c == d && (c == '|' || c == '&' || c == '<' || c == '>'))
		return (2);
	else if ((c == '|' || c == '<' || c == '>') && d != c)
		return (1);
	return (0);
}

int	count_tokens(char *input)
{
	int		i;
	int		amount;
	bool	in_quote;
	bool	in_bracket;

	i = 0;
	amount = 0;
	in_quote = false;
	in_bracket = false;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]) && !in_quote && !in_bracket)
			i++;
		if (input[i])
			amount++;
		while (input[i] && !ft_isspace(input[i]) || in_quote || in_bracket)
		{
			handle_quotes_brackets(input[i], &in_quote, &in_bracket);
			i++;
		}
	}
	return (amount);
}

char	*process_token(char *new_input, int *count, bool *flags, char **tokens)
{
	while (new_input[count[0]] && ft_isspace(new_input[count[0]])
		&& !flags[0] && !flags[1])
	{
		count[0]++;
	}
	count[1] = count[0];
	while ((new_input[count[1]] && (!ft_isspace(new_input[count[1]])
				|| flags[0] || flags[1])))
	{
		handle_quotes_brackets(new_input[count[1]], &flags[0], &flags[1]);
		count[1]++;
	}
	tokens[count[2]] = ft_substr(new_input, count[0], count[1] - count[0]);
	if (!tokens[count[2]])
	{
		while (count[2] >= 0)
			free(tokens[count[2]--]);
		free(tokens);
		return (free(new_input), NULL);
	}
	count[0] = count[1];
	count[2]++;
	return (new_input);
}

char	**tokenize(char *input)
{
	char	**tokens;
	char	*new_input;
	int		count[4];
	bool	flags[2];

	init_vars(count, flags);
	new_input = add_spaces(input);
	if (!new_input)
		return (NULL);
	count[3] = count_tokens(new_input);
	tokens = malloc(sizeof(char *) * (count[3] + 1));
	if (!tokens)
		return (free(new_input), NULL);
	while (count[2] < count[3])
	{
		new_input = process_token(new_input, count, flags, tokens);
		if (!new_input)
			return (NULL);
	}
	tokens[count[2]] = NULL;
	free(new_input);
	return (tokens);
}

// int main()
// {
// 	char **str = tokenize("'$USER' \"'jo'\"hallo\"\" $? "
// 				"ft_*.c hallo test <&&ls");
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