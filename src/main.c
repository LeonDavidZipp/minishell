/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:16:21 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/23 15:16:24 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*get_input(void);

int	main(int argc, char **argv, char **environ)
{
	char		*input;
	t_app_data	app_data;

	(void)argv;
	if (argc != 1)
	{
		printf("\033[0;31mUsage: ./minishell\033[0m\n");
		return (1);
	}
	app_data.env_vars = init_env_vars(environ);
	signal_handler();
	while (true)
	{
		app_data.input = get_input();
		if (app_data.input == NULL)
			continue ;
		lexer(&app_data);
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
