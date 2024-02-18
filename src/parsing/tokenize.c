/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:07:56 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/18 17:31:36 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	unsigned int	j;
	char			*ptr;

	if (s == NULL)
		return (NULL);
	if ((int)start >= ft_strlen(s))
	{
		ptr = malloc(1);
		if (ptr == NULL)
			return (NULL);
		ptr[0] = '\0';
		return (ptr);
	}
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	ptr = malloc(len + 1);
	if (ptr == NULL)
		return (NULL);
	i = start;
	j = 0;
	while (s[i] != '\0' && j < len)
		ptr[j++] = s[i++];
	ptr[j] = '\0';
	return (ptr);
}

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

// int	count_tokens(char *input)
// {
// 	int		i;
// 	int		j;
// 	int		amount;
// 	bool	in_quote;
// 	bool	in_bracket;

// 	i = 0;
// 	j = 0;
// 	amount = 0;
// 	in_quote = false;
// 	in_bracket = false;
// 	while (input[i])
// 	{
// 		handle_quotes_brackets(input[i], &in_quote, &in_bracket);
// 		if (input[i] && is_whitespace(input[i]) && !in_quote && !in_bracket)
// 		{
// 			if (j)
// 				j = 0;
// 		}
// 		else
// 		{
// 			if (!j)
// 			{
// 				amount++;
// 				j = 1;
// 			}
// 		}
// 		i++;
// 	}
// 	return (amount);
// }

int count_tokens(char *input)
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

void	noch_kein_name(char *input, int *j, int *i) // needs an actual name
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
			noch_kein_name(input, &j, &i);
		}
		i++;
	}
	return (j);
}

void	noch_kein_name_2(char *input, char *new_input, int *j, int *i) // needs an actual name
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
	new_input = malloc(sizeof(char) * ((ft_strlen(input) + new_input_length(input)) + 1));
	if (!new_input)
		return (NULL);
	while (input[i])
	{
		handle_quotes_brackets(input[i], &in_quote, &in_bracket);
		if (input[i] && input[i + 1] && is_operator(input[i], input[i + 1]) && !in_quote && !in_bracket)
			noch_kein_name_2(input, new_input, &j, &i);
		else
			new_input[j++] = input[i++];
	}
	new_input[j] = '\0';
	return (new_input);
}

char	**tokenize(char *input)
{
	char	**tokens;
	char	*new_input;
	int		i;
	int		j;
	int		k;
	bool	in_quote;
	bool	in_bracket;
	int		amount;

	i = 0;
	j = 0;
	k = 0;
	in_quote = false;
	in_bracket = false;
	new_input = add_spaces(input);
	if (!new_input)
		return (NULL);
	amount = count_tokens(new_input);
	tokens = malloc(sizeof(char *) * (amount + 1));
	if (!tokens)
		return (free(new_input), NULL);
	while (k < amount)
	{
		while (new_input[i] && is_whitespace(new_input[i]) && !in_quote && !in_bracket)
			i++;
		j = i;
		while ((new_input[j] && (!is_whitespace(new_input[j]) || in_quote || in_bracket)))
		{
			handle_quotes_brackets(new_input[j], &in_quote, &in_bracket);
			j++;
		}
		tokens[k] = ft_substr(new_input, i, j - i);
		if (!tokens[k])
		{
			while (k >= 0)
				free(tokens[k--]);
			free(tokens);
			return (free(new_input), NULL);
		}
		i = j;
		k++;
	}
	tokens[k] = NULL;
	free(new_input);
	return (tokens);
}

int main()
{
	//char **str = tokenize("lala hall cat|ls bla cat|ls 'hallo  \"hallo du $USER\" cat|ls (ich du) hallo > > test2 'this a longer test' du' was&& geht hallo \"hallo du $USER\" cat|ls (ich du) hallo > > test2 'this a longer test' \"even test with $VARIABLE\" | grep 'something' > output.txt (nested (brackets (test))) \"nested 'quotes' test\" 'nested \"quotes\" test' cat file1 && file2 file3 | sort | uniq > result.txt < input.txt");
	char **str = tokenize("'$USER' $? ft_*.c hallo test <&&ls");
	for (int i = 0; str[i]; i++)
	{
		printf("%s\n", str[i]);
	}
	return 0;
}