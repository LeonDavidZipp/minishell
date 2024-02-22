/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:07:56 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/22 19:15:43 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// int	ft_isspace(int c)
// {
// 	return (c == ' ' || c == '\t' || c == '\n'
// 		|| c == '\v' || c == '\f' || c == '\r');
// }

// size_t	ft_strlen(const char *s)
// {
// 	size_t	len;

// 	if (!s)
// 		return (0);
// 	len = 0;
// 	while (s[len])
// 		len++;
// 	return (len);
// }

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	size_t	j;
// 	size_t	s_len;
// 	char	*res;

// 	if (!s)
// 		return (NULL);
// 	j = 0;
// 	s_len = ft_strlen(s);
// 	while (j + start < s_len && s[j + start] && j < len)
// 		j++;
// 	res = (char *)malloc((j + 1) * sizeof(char));
// 	if (!res)
// 		return (NULL);
// 	j = 0;
// 	while (start < s_len && s[start] && len > 0)
// 	{
// 		res[j++] = s[start++];
// 		len--;
// 	}
// 	res[j] = '\0';
// 	return (res);
// }

// void	handle_quotes_brackets(char c, bool *s_quote, bool *d_quote, bool *in_bracket)
// {
// 	if (c == '\'' && !*d_quote && !*in_bracket)
// 		*s_quote = !*s_quote;
// 	if (c == '"' && !*s_quote && !*in_bracket)
// 		*d_quote = !*d_quote;
// 	if (c == '(')
// 		*in_bracket = true;
// 	if (c == ')')
// 		*in_bracket = false;
// }

// void	count_if_space_needed(char *input, int *j, int *i)
// {
// 	if (is_operator(input[*i], input[*i + 1]) == 2)
// 	{
// 		if (input[*i - 1] && !ft_isspace(input[*i - 1]))
// 			(*j)++;
// 		if (input[*i + 2] && !ft_isspace(input[*i + 2]))
// 			(*j)++;
// 	}
// 	else
// 	{
// 		if (input[*i - 1] && !ft_isspace(input[*i - 1]))
// 			(*j)++;
// 		if (input[*i + 1] && !ft_isspace(input[*i + 1]))
// 			(*j)++;
// 	}
// }

// int	new_input_length(char *input)
// {
// 	int		i;
// 	int		j;
// 	bool	s_quote;
// 	bool	d_quote;
// 	bool	in_bracket;

// 	i = 0;
// 	j = 0;
// 	s_quote = false;
// 	d_quote = false;
// 	in_bracket = false;
// 	while (input[i])
// 	{
// 		handle_quotes_brackets(input[i], &s_quote, &d_quote, &in_bracket);
// 		if (input[i] && input[i + 1] && is_operator(input[i], input[i + 1])
// 			&& !s_quote && !d_quote && !in_bracket)
// 		{
// 			count_if_space_needed(input, &j, &i);
// 		}
// 		i++;
// 	}
// 	return (j);
// }

// void	check_if_space_needed(char *input, char *new_input, int *j, int *i)
// {
// 	if (is_operator(input[*i], input[*i + 1]) == 2)
// 	{
// 		if (input[*i - 1] && !ft_isspace(input[*i - 1]))
// 			new_input[(*j)++] = ' ';
// 		new_input[(*j)++] = input[(*i)++];
// 		new_input[(*j)++] = input[(*i)++];
// 		if (!ft_isspace(input[*i]))
// 			new_input[(*j)++] = ' ';
// 	}
// 	else
// 	{
// 		if (input[*i - 1] && !ft_isspace(input[*i - 1]))
// 			new_input[(*j)++] = ' ';
// 		new_input[(*j)++] = input[(*i)++];
// 		if (!ft_isspace(input[*i]))
// 			new_input[(*j)++] = ' ';
// 	}
// }

// char	*add_spaces(char *input)
// {
// 	char	*new_input;
// 	int		i;
// 	int		j;
// 	bool	s_quote;
// 	bool	d_quote;
// 	bool	in_bracket;

// 	i = 0;
// 	j = 0;
// 	s_quote = false;
// 	d_quote = false;
// 	in_bracket = false;
// 	new_input = malloc(sizeof(char) * ((ft_strlen(input)
// 					+ new_input_length(input)) + 1));
// 	if (!new_input)
// 		return (NULL);
// 	while (input[i])
// 	{
// 		handle_quotes_brackets(input[i], &s_quote, &d_quote, &in_bracket);
// 		if (input[i] && input[i + 1] && is_operator(input[i], input[i + 1])
// 			&& !s_quote && !d_quote && !in_bracket)
// 			check_if_space_needed(input, new_input, &j, &i);
// 		else
// 			new_input[j++] = input[i++];
// 	}
// 	new_input[j] = '\0';
// 	return (new_input);
// }

void	init_vars(int *count, bool *flags)
{
	count[0] = 0;
	count[1] = 0;
	count[2] = 0;
	flags[0] = false;
	flags[1] = false;
	flags[2] = false;
}

int	is_operator(char c, char d)
{
	if (c == d && (c == '|' || c == '&' || c == '<' || c == '>'))
		return (2);
	else if ((c == '|' || c == '<' || c == '>') && d != c)
		return (1);
	return (0);
}

// int	count_tokens(char *input)
// {
// 	int		i;
// 	int		amount;
// 	bool	s_quote;
// 	bool	d_quote;
// 	bool	in_bracket;

// 	i = 0;
// 	amount = 0;
// 	s_quote = false;
// 	d_quote = false;
// 	in_bracket = false;
// 	while (input[i])
// 	{
// 		while (input[i] && ft_isspace(input[i]) && !s_quote && !d_quote && !in_bracket)
// 			i++;
// 		if (input[i])
// 			amount++;
// 		while (input[i] && (!ft_isspace(input[i]) || s_quote || d_quote || in_bracket))
// 		{
// 			handle_quotes_brackets(input[i], &s_quote, &d_quote, &in_bracket);
// 			i++;
// 		}
// 	}
// 	return (amount);
// }

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
		handle_quotes_brackets(input[i], &flags[0], &flags[1], &flags[2]);
		if ((!flags[0] && !flags[1] && !flags[2] && !ft_isspace(input[i])) && 
			(i == 0 || ft_isspace(input[i-1]) || input[i-1] == '\''
			|| input[i-1] == '\"' || input[i-1] == '(' || was_flag))
		{
			amount++;
		}
		was_flag = flags[0] || flags[1] || flags[2];
		i++;
	}
	return (amount);
}

// need to update process_token to handle nested quotes and brackets correctly
char	*process_token(char *new_input, int *count, bool *flags, char **tokens)
{
	while (new_input[count[0]] && ft_isspace(new_input[count[0]])
		&& !flags[0] && !flags[1] && !flags[2])
	{
		count[0]++;
	}
	count[1] = count[0];
	while ((new_input[count[1]] && (!ft_isspace(new_input[count[1]])
				|| flags[0] || flags[1] || flags[2])))
	{
		handle_quotes_brackets(new_input[count[1]], &flags[0], &flags[1], &flags[2]);
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
	bool	flags[3];

	init_vars(count, flags);
	new_input = add_spaces(input);
	if (!new_input)
		return (NULL);
	count[3] = count_tokens(new_input);
	printf("count[3]: %d\n", count[3]);
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

int main()
{
	char **str = tokenize("'\"'\"hello\"'\"'");
	for (int i = 0; str[i]; i++)
	{
		printf("%s\n", str[i]);
	}
	for (int i = 0; str[i]; i++)
	{
		free(str[i]);
	}
	free(str);
	return 0;
}