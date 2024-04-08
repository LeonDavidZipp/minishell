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

int	process_dir_entries(DIR *dir, char *pattern, char **output, int *i)
{
	bool			flag;
	struct dirent	*entry;

	flag = false;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.' || (entry->d_name[0] != '.' && entry->d_name[1] != '.'))
		{
			if (match(pattern, entry->d_name))
			{
				i[2] = 0;
				flag = true;
				while (entry->d_name[i[2]])
					(*output)[i[1]++] = entry->d_name[i[2]++];
				(*output)[i[1]++] = ' ';
			}
		}
		entry = readdir(dir);
	}
	if (flag)
		return (1);
	return (0);
}

int	handle_wildcard(char *input, char **output, int *i)
{
	DIR				*dir;
	char			*pattern;
	int				position;
	bool			flag;
	int				start;

	flag = false;
	start = 0;
	pattern = get_pattern(input, &i[0], &position, &start);
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
	if (!process_dir_entries(dir, pattern, output, i))
	{
		while(!is_space(input[start]))
			(*output)[i[1]++] = input[start++];
		i[1]++;
	}
	closedir(dir);
	free(pattern);
	return (1);
}

// if ((data->input[data->i[0]] == '$' && !data->quotes[0] && is_valid_dollar(data->input, data->i[0])));
// handle in quotes
// if ((data->input[data->i[0]] == '~' && !data->quotes[0] && !data->quotes[1]));
// if ((data->input[data->i[0]] == '~')
// handle before
// 		&& ((data->i[0] == 0 || ft_isspace(data->input[data->i[0] - 1]))
// handle after
// 			&& ((data->i[0] < ft_strlen(data->input) - 1 && (ft_isspace(data->input[data->i[0] + 1]) || data->input[data->i[0] + 1] == '\0')))))

void	handle_character(t_expand *data)
{
	handle_quotes(data->input[data->i[0]], &data->quotes[0], &data->quotes[1]);
	if ((data->input[data->i[0]] == '$' && !data->quotes[0] && is_valid_dollar(data->input, data->i[0], data->quotes))
		|| (data->input[data->i[0]] == '~' && !data->quotes[0] && !data->quotes[1]
			&& (is_space(data->input[data->i[0] + 1]) || data->input[data->i[0] + 1] == '/')))
	{
		if (!handle_dollar(data))
			return ;
	}
	else if (data->input[data->i[0]] == '*' && !data->quotes[0]
		&& !data->quotes[1] && data->flag == 0)
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
