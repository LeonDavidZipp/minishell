/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:07:56 by cgerling          #+#    #+#             */
/*   Updated: 2024/03/26 11:12:25 by lzipp            ###   ########.fr       */
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
}

int	count_tokens(char *input)
{
	int		i;
	int		amount;
	bool	flags[2];

	i = 0;
	amount = 0;
	flags[0] = false;
	flags[1] = false;
	while (input[i])
	{
		while (input[i] && is_space(input[i]) && !flags[0]
			&& !flags[1])
			i++;
		if (input[i])
			amount++;
		while (input[i] && (!is_space(input[i]) || flags[0]
				|| flags[1]))
		{
			handle_quotes(input[i], &flags[0], &flags[1]);
			i++;
		}
	}
	return (amount);
}

int	process_token(char *input, int *count, bool *flags, char **tokens)
{
	while (input[count[0]] && is_space(input[count[0]])
		&& !flags[0] && !flags[1])
		count[0]++;
	count[1] = count[0];
	while ((input[count[1]] && (!is_space(input[count[1]])
				|| flags[0] || flags[1])))
	{
		handle_quotes(input[count[1]], &flags[0], &flags[1]);
		count[1]++;
	}
	tokens[count[2]] = ft_substr(input, count[0], count[1] - count[0]);
	if (!tokens[count[2]])
		return (ft_free_2d_arr((void **)tokens), free(input), 0);
	count[0] = count[1];
	count[2]++;
	return (1);
}

char	**split(char *input, int exit_code)
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
		return (ft_fprintf(2, "%s: malloc: %s\n", NAME, strerror(errno)),
			free(new_input), NULL);
	tokens[count[3]] = NULL;
	while (count[2] < count[3])
	{
		if (!process_token(new_input, count, flags, tokens))
			return (free(new_input), free(tokens), NULL);
	}
	free(new_input);
	return (expand_and_remove(tokens, exit_code));
}

// int main()
// {
// 	char *input = "echo \"> >> < * ? [ ] | ; [ ] || && ( ) $  <<\"";
// 	if (check_input(input) == 1)
// 		return (1);
// 	char **tokens = split(input, 0);
// 	if (!tokens)
// 		return 1;
// 	for (int i = 0; tokens[i]; i++)
// 	{
// 		printf("%s\n", tokens[i]);
// 	}
// 	for (int i = 0; tokens[i]; i++)
// 	{
// 		free(tokens[i]);
// 	}
// 	free(tokens);
// 	return 0;
// }
