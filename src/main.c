/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:22:58 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/20 10:54:15 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*get_input(void);

int	main(int argc, char **argv, char **environ)
{
	char		*input;
	t_env_var	*env_vars;

	(void)argv;
	if (argc != 1)
	{
		printf("\033[0;31mUsage: ./minishell\033[0m\n");
		return (1);
	}
	env_vars = init_env_vars(environ);
	signal_handler();
	while (true)
	{
		input = get_input();
		if (input == NULL)
			continue ;
	}
	return (0);
}

static char	*get_input(void)
{
	char	*input;

	input = readline(PROMPT);
	if (input == NULL)
	{
		printf("exit\n");
		// free memory!!!!!!!!!!!!!!!!
		// free memory!!!!!!!!!!!!!!!!
		// free memory!!!!!!!!!!!!!!!!
		exit(0);
	}
	else if (ft_strlen(input) == 0)
	{
		free(input);
		return (NULL);
	}
	else if (ft_strncmp(input, "exit", ft_strlen(input)) == 0)
	{
		add_history(input);
		free(input);
		printf("exit\n");
		// free memory!!!!!!!!!!!!!!!!
		// free memory!!!!!!!!!!!!!!!!
		// free memory!!!!!!!!!!!!!!!!
		exit(0);
	}
	add_history(input);
	return (input);
}
