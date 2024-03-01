/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:24:57 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 11:14:35 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*get_input(void);
static void	print_logo(void);

int	main(int argc, char **argv, char **envp)
{
	t_app_data	app_data;

	(void)argv;
	if (argc != 1)
	{
		printf("\033[0;31mUsage: ./minishell\033[0m\n");
		return (1);
	}
	app_data.env_vars = init_envp(envp);
	signal_handler();
	print_logo();
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
		// maybe change exit code
		builtin_exit(0);
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
		// maybe change exit code
		builtin_exit(0);
	}
	add_history(input);
	return (input);
}

static void	print_logo(void)
{
	char	*lzipp;
	char	*cgerling;

	// font is ANSI Shadow
	lzipp = "https://profile.intra.42.fr/users/lzipp";
	cgerling = "https://profile.intra.42.fr/users/cgerling";
	printf("\033[0;32m\n\n");
	printf("	██████╗  █████╗ ██████╗  █████╗ ███████╗██╗  ██╗\n");
	printf("	██╔══██╗██╔══██╗██╔══██╗██╔══██╗██╔════╝██║  ██║\n");
	printf("	██████╔╝███████║██████╔╝███████║███████╗███████║\n");
	printf("	██╔══██╗██╔══██║██╔══██╗██╔══██║╚════██║██╔══██║\n");
	printf("	██████╔╝██║  ██║██████╔╝██║  ██║███████║██║  ██║\n");
	printf("	╚═════╝ ╚═╝  ╚═╝╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝");
	printf("\033[0;33m\n\n");
	printf("		   born again born again shell\n\n");
	printf("\033[0;36m			");
	printf("\e]8;;%s\alzipp\e]8;;\a && ", lzipp);
	printf("\e]8;;%s\acgerling\e]8;;\a\n", cgerling);
	printf("\033[0m\n");
}
