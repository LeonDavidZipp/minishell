/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:07:56 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/19 16:42:09 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\0')
		return (1);
	return (0);
}

int	is_operator(char c, char d)
{
	if (c == d && (c == '|' || c == '&' || c == '<' || c == '>'))
		return (2);
	else if ((c == '|' || c == '<' || c == '>') && d != c)
		return (1);
	return (0);
}

void	handle_quotes_brackets(char c, bool *in_quote, bool *in_bracket)
{
	if (c == '\'' || c == '\"')
		*in_quote = !*in_quote;
	if (c == '(')
		*in_bracket = true;
	if (c == ')')
		*in_bracket = false;
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
		while (input[i] && is_whitespace(input[i]) && !in_quote && !in_bracket)
			i++;
		if (input[i])
			amount++;
		while (input[i] && !is_whitespace(input[i]) || in_quote || in_bracket)
		{
			handle_quotes_brackets(input[i], &in_quote, &in_bracket);
			i++;
		}
	}
	return (amount);
}

void	count_if_space_needed(char *input, int *j, int *i)
{
	if (is_operator(input[*i], input[*i + 1]) == 2)
	{
		if (input[*i - 1] && !is_whitespace(input[*i - 1]))
			(*j)++;
		if (input[*i + 2] && !is_whitespace(input[*i + 2]))
			(*j)++;
	}
	else
	{
		if (input[*i - 1] && !is_whitespace(input[*i - 1]))
			(*j)++;
		if (input[*i + 1] && !is_whitespace(input[*i + 1]))
			(*j)++;
	}
}

int	new_input_length(char *input)
{
	char	*new_input;
	int		i;
	int		j;
	bool	in_quote;
	bool	in_bracket;

	i = 0;
	j = 0;
	in_quote = false;
	in_bracket = false;
	while (input[i])
	{
		handle_quotes_brackets(input[i], &in_quote, &in_bracket);
		if (input[i] && input[i + 1] && is_operator(input[i], input[i + 1])
			&& !in_quote && !in_bracket)
		{
			count_if_space_needed(input, &j, &i);
		}
		i++;
	}
	return (j);
}

void	check_if_space_needed(char *input, char *new_input, int *j, int *i)
{
	if (is_operator(input[*i], input[*i + 1]) == 2)
	{
		if (input[*i - 1] && !is_whitespace(input[*i - 1]))
			new_input[(*j)++] = ' ';
		new_input[(*j)++] = input[(*i)++];
		new_input[(*j)++] = input[(*i)++];
		if (!is_whitespace(input[*i]))
			new_input[(*j)++] = ' ';
	}
	else
	{
		if (input[*i - 1] && !is_whitespace(input[*i - 1]))
			new_input[(*j)++] = ' ';
		new_input[(*j)++] = input[(*i)++];
		if (!is_whitespace(input[*i]))
			new_input[(*j)++] = ' ';
	}
}

char	*add_spaces(char *input)
{
	char	*new_input;
	int		i;
	int		j;
	bool	in_quote;
	bool	in_bracket;

	i = 0;
	j = 0;
	in_quote = false;
	in_bracket = false;
	new_input = malloc(sizeof(char) * ((ft_strlen(input)
					+ new_input_length(input)) + 1));
	if (!new_input)
		return (NULL);
	while (input[i])
	{
		handle_quotes_brackets(input[i], &in_quote, &in_bracket);
		if (input[i] && input[i + 1] && is_operator(input[i], input[i + 1])
			&& !in_quote && !in_bracket)
			check_if_space_needed(input, new_input, &j, &i);
		else
			new_input[j++] = input[i++];
	}
	new_input[j] = '\0';
	return (new_input);
}

static void	init_vars(int *count, bool *flags)
{
	count[0] = 0;
	count[1] = 0;
	count[2] = 0;
	flags[0] = false;
	flags[1] = false;
}

char	*process_token(char *new_input, int *count, bool *flags, char **tokens)
{
	while (new_input[count[0]] && is_whitespace(new_input[count[0]])
		&& !flags[0] && !flags[1])
	{
		count[0]++;
	}
	count[1] = count[0];
	while ((new_input[count[1]] && (!is_whitespace(new_input[count[1]])
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
// 	//char **str = tokenize("lala hall cat|ls bla cat|ls hallo  \"hallo du $USER\" cat|ls (ich du) hallo > > test2 'this a longer test' du was&& geht hallo \"hallo du $USER\" cat|ls (ich du) hallo > > test2 'this a longer test' \"even test with $VARIABLE\" | grep 'something' > output.txt (nested (brackets (test))) \"nested 'quotes' test\" 'nested \"quotes\" test' cat file1 && file2 file3 | sort | uniq > result.txt < input.txt");
// 	char **str = tokenize("'$USER' \"'jo'\"hallo\"\" $? ft_*.c hallo test <&&ls 'another$USER' \"'moretext'\"more\"\" $? more*.c more test <&&more 'yetanother$USER' \"'evenmoretext'\"evenmore\"\" $? evenmore*.c evenmore test <&&evenmore");
// 	//char **str = tokenize("'$USER' \"'jo'\"hallo\"\" $? ft_*.c hallo test <&&ls");
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