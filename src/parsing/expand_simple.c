/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 14:59:08 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/19 16:39:14 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strndup(const char *s1, size_t n)
{
	char	*dest;
	size_t	i;

	if (!s1)
		return (NULL);
	i = 0;
	while (s1[i] && i < n)
		i++;
	dest = (char *)malloc((i + 1) * sizeof (char));
	if (!dest)
		return (NULL);
	i = 0;
	while (s1[i] && i < n)
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

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
	name = ft_strndup(input + 1, i - 1);
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

char	**expand_wildcard(char *input)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**output;
	int				i;

	i = 0;
	output = (char **)ft_calloc(100, sizeof(char *)); // need to change 100
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