/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 14:59:08 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/20 10:35:58 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\0')
		return (1);
	return (0);
}

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

char	*expand_exit_code(int last_exit_code)
{
	char	*exit_code;

	exit_code = ft_itoa(last_exit_code);
	if (!exit_code)
		return (NULL);
	return (exit_code);
}

char	*expand_var(char *input, char **envp)
{
	char	*name;
	char	*value;
	char	*output;
	int		i;

	i = 1;
	while (ft_isalnum(input[i]) || input[i] == '_')
		i++;
	// name = ft_strndup(input + 1, i - 1);
	name = ft_substr(input, 1, i - 1);
	if (!name)
		return (NULL);
	value = getenv(name);
	if (value)
	{
		output = ft_strdup(value);
		if (!output)
			return (NULL);
		free(name);
		return (output);
	}
	free(name);
	return (NULL);
}

int	size(char *input)
{
	DIR				*dir;
	struct dirent	*entry;
	int				i;

	i = 0;
	dir = opendir(".");
	if (dir == NULL)
		return (0);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (match(input, entry->d_name))
			i++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (i);
}

char	**expand_wildcard(char *input)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**output;
	int				i;

	i = 0;
	output = (char **)ft_calloc((size(input) + 1), sizeof(char *));
	if (!output)
		return (NULL);
	dir = opendir(".");
	if (dir == NULL)
		return (NULL);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (match(input, entry->d_name))
			output[i++] = ft_strdup(entry->d_name);
		entry = readdir(dir);
	}
	if (i == 0)
		return (free(output), NULL);
	output[i] = NULL;
	closedir(dir);
	return (output);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	char	*input = "*";
// 	int		i = 0;
// 	if (input[i] == '$' && input[i + 1] == '?')
// 	{
// 		char *exit_code = expand_exit_code(0); // 0 for testing
// 		printf("%s\n", exit_code); // 0 for testing
// 		free(exit_code);
// 	}
// 	else if (input[i] == '$')
// 	{
// 		char *output = expand_var(input, envp);
// 		if (output)
// 		{
// 			printf("%s\n", output);
// 			free(output);
// 		}
// 	}
// 	else
// 	{
// 		char **output = expand_wildcard(input);
// 		int j = 0;
// 		while (output[j])
// 		{
// 			printf("%s\n", output[j]);
// 			free(output[j]);
// 			j++;
// 		}
// 		free(output);
// 	}
// 	return (0);
// }