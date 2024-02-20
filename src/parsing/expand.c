/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 10:43:26 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/20 10:47:13 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	match(char *pattern, char *string)
{
	while (*pattern && *string)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			while (*string)
			{
				if (match(pattern, string))
					return (true);
				string++;
			}
			return (match(pattern, string));
		}
		else if (*pattern == *string)
		{
			pattern++;
			string++;
		}
		else
			return (false);
	}
	return (*pattern == *string);
}

int	env_var_size(char *input, int *i)
{
	int		start;
	int		size;
	char	*name;
	char	*value;

	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	// name = ft_strndup(input + start, *i - start);
	name = ft_substr(input, start, *i - start);
	if (!name)
		return (0);
	value = getenv(name);
	if (value)
		size = ft_strlen(value);
	else
		size = 0;
	free(name);
	return (size);
}

int	get_new_size(char *input, int last_exit_code)
{
	int	i;
	int	size;
	int	start;

	i = 0;
	size = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '?')
		{
			size += ft_dec_len(last_exit_code);
			i += 2;
		}
		else if (input[i] == '$')
		{
			i++;
			size += env_var_size(input, &i);
		}
		else
		{
			size++;
			i++;
		}
	}
	return (size);
}

int	exit_code(char **str, int *j, int last_exit_code)
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

int	env_var(char *input, char **envp, char **output, int *j)
{
	char	*name;
	char	*value;
	int		i;

	i = 1;
	while (ft_isalnum(input[i]) || input[i] == '_')
		i++;
	// name = ft_strndup(input + 1, i - 1);
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

char	*handle_expansion(char *input, char **output, int *i, char **envp, int last_exit_code)
{
	if (input[i[0]] == '$' && input[i[0] + 1] == '?')
	{
		if (!exit_code(output, &i[1], last_exit_code))
			return (free(*output), NULL);
		i[0] += 2;
	}
	else if (input[i[0]] == '$')
	{
		if (!env_var(input + i[0], envp, output, &i[1]))
			return (free(*output), NULL);
		i[2] = 1;
		while (ft_isalnum(input[i[0] + i[2]]) || input[i[0] + i[2]] == '_')
			i[2]++;
		i[0] += i[2];
	}
	return (*output);
}

// last exit code should be in a struct, same as envp (they should be in the same struct otherwise they two parsing arguments which is shit)

char	*in_string_expansion(char *input, char **envp, int last_exit_code)
{
	char	*output;
	int		i[3];

	i[0] = 0;
	i[1] = 0;
	output = (char *)ft_calloc((get_new_size(input, last_exit_code) + 1), sizeof(char));
	printf("%d\n", get_new_size(input, last_exit_code));
	if (!output)
		return (NULL);
	while (input[i[0]])
	{
		if (input[i[0]] == '$')
		{
			output = handle_expansion(input, &output, i, envp, last_exit_code);
			if (!output)
				return (NULL);
		}
		else
			output[i[1]++] = input[i[0]++];
	}
	output[i[1]] = '\0';
	return (output);
}

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