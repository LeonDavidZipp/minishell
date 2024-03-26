/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 10:43:26 by cgerling          #+#    #+#             */
/*   Updated: 2024/03/26 13:03:35 by cgerling         ###   ########.fr       */
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

void	handle_character(t_expand *data)
{
	handle_quotes(data->input[data->i[0]], &data->quotes[0], &data->quotes[1]);
	if (data->input[data->i[0]] == '$'
		&& !ft_isspace(data->input[data->i[0] + 1])
		&& !(ft_isalnum(data->input[data->i[0] + 1])
			|| data->input[data->i[0] + 1] == '_'
			|| data->input[data->i[0] + 1] == '?'))
	{
		(*data->output)[data->i[1]++] = data->input[data->i[0]++];
		return ;
	}
	if (data->input[data->i[0]] == '$' && !data->quotes[0])
	{
		if (!handle_dollar(data))
			return ;
	}
	else if (data->input[data->i[0]] == '*' && !data->quotes[0]
		&& !data->quotes[1] && data->i[3] == 0)
	{
		if (!handle_wildcard(data->input, data->output, data->i))
			return ;
		data->i[1]--;
	}
	else
		(*data->output)[data->i[1]++] = data->input[data->i[0]++];
}

static	void	init_vars(int *i, bool *quotes)
{
	i[0] = 0;
	i[1] = 0;
	quotes[0] = false;
	quotes[1] = false;
}

char	*expand(char *input, int exit_code, char **env_vars, int flag)
{
	char		*output;
	int			size;
	int			i[3];
	bool		quotes[2];
	t_expand	data;

	init_vars(i, quotes);
	data.env_vars = env_vars;
	data.flag = flag;
	data.exit_code = exit_code;
	data.i = i;
	data.quotes = quotes;
	data.input = input;
	size = get_new_size(input, exit_code, env_vars, flag);
	output = (char *)ft_calloc((size + 1), sizeof(char));
	if (!output)
		return (NULL);
	data.output = &output;
	while (input[i[0]])
	{
		handle_character(&data);
	}
	if (output != NULL)
		output[i[1]] = '\0';
	return (output);
}

// int	main()
// {
// 	// char	*input = "hello $USER your number is $? * test"; // testing
// 	char	*input = "*.c"; // testing
// 	char	*output = data(input, 0);
// 	if (output)
// 	{
// 		printf("%s\n", output);
// 		free(output);
// 	}
// 	return (0);
// }
