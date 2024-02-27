/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 10:43:26 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/27 19:21:55 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exit_code_expand(char **str, int *j, int last_exit_code)
{
	char	*exit_code;
	int		i;

	exit_code = ft_itoa(last_exit_code);
	if (!exit_code)
		return (0);
	i = 0;
	while (exit_code[i])
	{
		(*str)[*j] = exit_code[i];
		(*j)++;
		i++;
	}
	free(exit_code);
	return (1);
}

int	var_expand(char *input, char **output, int *j)
{
	char	*name;
	char	*value;
	int		i;

	i = 1;
	while (ft_isalnum(input[i]) || input[i] == '_')
		i++;
	name = ft_substr(input, 1, i - 1);
	if (!name)
		return (0);
	value = getenv(name);
	if (value)
	{
		i = 0;
		while (value[i])
			(*output)[(*j)++] = value[i++];
		free(name);
	}
	else
		free(name);
	return (1);
}

int	handle_dollar(char *input, char **output, int *i, t_app_data *app)
{
	if (input[i[0]] == '$' && input[i[0] + 1] == '?')
	{
		if (!exit_code_expand(output, &i[1], app->last_exit_code))
			return (free(*output), 0);
		i[0] += 2;
	}
	else if (input[i[0]] == '$')
	{
		if (!var_expand(input + i[0], output, &i[1]))
			return (free(*output), 0);
		i[2] = 1;
		while (ft_isalnum(input[i[0] + i[2]]) || input[i[0] + i[2]] == '_')
			i[2]++;
		i[0] += i[2];
	}
	return (1);
}

void	handle_quotes(char c, bool *s_quote, bool *d_quote)
{
	if (c == '\'' && !*d_quote)
		*s_quote = !*s_quote;
	else if (c == '"' && !*s_quote)
		*d_quote = !*d_quote;
}

char	*expand(char *input, t_app_data *app)
{
	char	*output;
	int		size;
	int		i[2];
	bool	s_quote;
	bool	d_quote;

	i[0] = 0;
	i[1] = 0;
	s_quote = false;
	d_quote = false;
	size = get_new_size(input, app->last_exit_code); // TODO: modify get_new_size to handle quotes and wildcards
	output = (char *)ft_calloc((size + 1), sizeof(char));
	if (!output)
		return (NULL);
	while (input[i[0]])
	{
		handle_quotes(input[i[0]], &s_quote, &d_quote);
		if (input[i[0]] == '$' && !s_quote)
		{
			if (!handle_dollar(input, &output, i, app))
				return (NULL);
		}
		else if (input[i[0]] == '*' && !s_quote && !d_quote)
		{
			if (!handle_wildcard(input, &output, i, app)) // TODO: implement handle_wildcard
				return (NULL);
		}
		else
			output[i[1]++] = input[i[0]++];
	}
}

// char	*in_string_expansion(char *input, t_app_data *app)
// {
// 	char	*output;
// 	int		size;
// 	int		i[3];

// 	i[0] = 0;
// 	i[1] = 0;
// 	size = get_new_size(input, app->last_exit_code);
// 	output = (char *)ft_calloc((size + 1), sizeof(char));
// 	if (!output)
// 		return (NULL);
// 	while (input[i[0]])
// 	{
// 		if (input[i[0]] == '$')
// 		{
// 			output = handle_expansion(input, &output, i, app);
// 			if (!output)
// 				return (NULL);
// 		}
// 		else
// 			output[i[1]++] = input[i[0]++];
// 	}
// 	output[i[1]] = '\0';
// 	return (output);
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	char	*input = "hello $HAALO your number is $?"; // testing
// 	int		i = 0;
// 	char *output = in_string_expansion(input, envp, 0);
// 	if (output)
// 	{
// 		printf("%s\n", output);
// 		free(output);
// 	}
// 	return (0);
// }
