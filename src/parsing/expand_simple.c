/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 14:59:08 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/27 18:20:28 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_exit_code(int last_exit_code)
{
	char	*exit_code;

	exit_code = ft_itoa(last_exit_code);
	if (!exit_code)
		return (NULL);
	return (exit_code);
}

char	*expand_var(char *input)
{
	char	*name;
	char	*value;
	char	*output;
	int		i;

	i = 1;
	while (ft_isalnum(input[i]) || input[i] == '_')
		i++;
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

char	*expand_wildcard(char *input)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*output;
	char			*temp;

	output = NULL;
	dir = opendir(".");
	if (dir == NULL)
		return (NULL);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (match(input, entry->d_name))
		{
			if (output)
			{
				temp = ft_strjoin(output, " ");
				free(output);
				output = ft_strjoin(temp, entry->d_name);
				free(temp);
			}
			else
			{
				output = ft_strdup(entry->d_name);
			}
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (output);
}

// int	main(void)
// {
// 	char	*input = "e*.c";
// 	int		i = 0;
// 	if (input[i] == '$' && input[i + 1] == '?')
// 	{
// 		char *exit_code = expand_exit_code(0); // 0 for testing
// 		printf("%s\n", exit_code); // 0 for testing
// 		free(exit_code);
// 	}
// 	else if (input[i] == '$')
// 	{
// 		char *output = expand_var(input);
// 		if (output)
// 		{
// 			printf("%s\n", output);
// 			free(output);
// 		}
// 	}
// 	else
// 	{
// 		char *output = expand_wildcard(input);
// 		if (!output)
// 			return (1);
// 		printf("%s\n", output);
// 		free(output);
// 	}
// 	return (0);
// }
