/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 10:43:26 by cgerling          #+#    #+#             */
/*   Updated: 2024/03/21 17:31:48 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	process_dir_entries(DIR *dir, char *pattern, char **output, int *i)
{
	struct dirent	*entry;

	entry = readdir(dir);
	while (entry != NULL)
	{
		if (match(pattern, entry->d_name))
		{
			i[2] = 0;
			while (entry->d_name[i[2]])
				(*output)[i[1]++] = entry->d_name[i[2]++];
			(*output)[i[1]++] = ' ';
		}
		entry = readdir(dir);
	}
}

int	handle_wildcard(char *input, char **output, int *i)
{
	DIR				*dir;
	char			*pattern;
	int				position;
	bool			flag;

	flag = false;
	pattern = get_pattern(input, &i[0], &position);
	if (!pattern)
		return (free(output), 0);
	dir = opendir(".");
	if (dir == NULL)
		return (free(output), 0);
	if (!flag)
	{
		i[1] -= position;
		flag = true;
	}
	process_dir_entries(dir, pattern, output, i);
	closedir(dir);
	free(pattern);
	return (1);
}

void	handle_character(char *input, char **output, int *i, bool *quotes)
{
	handle_quotes(input[i[0]], &quotes[0], &quotes[1]);
	if (input[i[0]] == '$' && !quotes[0])
	{
		if (!handle_dollar(input, output, i))
			return ;
	}
	else if (input[i[0]] == '*' && !quotes[0] && quotes[1] && i[3] == 0)
	{
		if (!handle_wildcard(input, output, i))
			return ;
		i[1]--;
	}
	else
		(*output)[i[1]++] = input[i[0]++];
}

char	*expand(char *input, int exit_code, int flag)
{
	char	*output;
	int		size;
	int		i[5];
	bool	quotes[2];

	i[0] = 0;
	i[1] = 0;
	i[3] = flag;
	i[4] = exit_code;
	quotes[0] = false;
	quotes[1] = false;
	size = get_new_size(input, exit_code, flag);
	output = (char *)ft_calloc((size + 1), sizeof(char));
	if (!output)
		return (NULL);
	while (input[i[0]])
	{
		handle_character(input, &output, i, quotes);
	}
	if (output != NULL)
		output[i[1]] = '\0';
	return (output);
}

// int	main()
// {
// 	// char	*input = "hello $USER your number is $? * test"; // testing
// 	char	*input = "*.c"; // testing
// 	char	*output = expand(input, 0);
// 	if (output)
// 	{
// 		printf("%s\n", output);
// 		free(output);
// 	}
// 	return (0);
// }
