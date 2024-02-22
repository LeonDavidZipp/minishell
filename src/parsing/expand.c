/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 10:43:26 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/20 11:46:55 by cgerling         ###   ########.fr       */
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

int	var_expand(char *input, t_env_var *env, char **output, int *j)
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

char	*handle_expansion(char *input, char **output, int *i, t_app_data *app)
{
	if (input[i[0]] == '$' && input[i[0] + 1] == '?')
	{
		if (!exit_code(output, &i[1], app->last_exit_code))
			return (free(*output), NULL);
		i[0] += 2;
	}
	else if (input[i[0]] == '$')
	{
		if (!env_var(input + i[0], app->env_vars, output, &i[1]))
			return (free(*output), NULL);
		i[2] = 1;
		while (ft_isalnum(input[i[0] + i[2]]) || input[i[0] + i[2]] == '_')
			i[2]++;
		i[0] += i[2];
	}
	return (*output);
}

char	*in_string_expansion(char *input, t_app_data *app)
{
	char	*output;
	int		size;
	int		i[3];

	i[0] = 0;
	i[1] = 0;
	size = get_new_size(input, app->last_exit_code);
	output = (char *)ft_calloc((size + 1), sizeof(char));
	printf("%d\n", get_new_size(input, app->last_exit_code));
	if (!output)
		return (NULL);
	while (input[i[0]])
	{
		if (input[i[0]] == '$')
		{
			output = handle_expansion(input, &output, i, app);
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
