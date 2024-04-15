/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:24:57 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/15 19:00:33 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	init_app_data(t_app_data *app_data, char **envp);
static void	get_input(t_app_data *app_data);
// static void	print_logo(void);

int	g_exit_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_app_data	app_data;

	(void)argv;
	if (argc != 1)
	{
		// ft_fprintf(2, "\033[0;31mUsage: ./minishell\033[0m\n");
		return (1);
	}
	init_app_data(&app_data, envp);
	signal_handler();
	// print_logo();
	while (true)
	{
		// printf("first exit code: %d\n", app_data.last_exit_code);
		g_exit_signal = 0;
		get_input(&app_data);
		if (!isatty(fileno(stdin)) && app_data.input == NULL)
			break ;
		if (app_data.input == NULL)
			continue ;
		// printf("input: %s\n", app_data.input);
		lexer(&app_data);
		// printf("second exit code: %d\n", app_data.last_exit_code);
	}
	// printf("before return exit code: %d\n", app_data.last_exit_code);
	return (app_data.last_exit_code);
}

static void	get_input(t_app_data *app_data)
{
	char	*input;

	if (isatty(fileno(stdin)))
		input = readline(PROMPT);
	else
	{
		char *line;
		line = get_next_line(fileno(stdin));
		if (line == NULL)
			return ;
		input = ft_strtrim(line, "\n");
		free(line);
	}
	// input = readline(PROMPT);
	if (input && ft_strlen(input) == 0)
	{
		free(input);
		return ;
	}
	else if (input == NULL)
	{
		free(input);
		builtin_exit(app_data, NULL);
	}
	else if (ft_str_isspaces(input))
	{
		free(input);
		return ;
	}
	add_history(input);
	app_data->input = input;
}
static void	init_app_data(t_app_data *app_data, char **envp)
{
	app_data->env_vars = init_envp(envp);
	app_data->last_exit_code = 0;
	app_data->input = NULL;
	app_data->noninteractive = !isatty(fileno(stdin));
}

// static void	print_logo(void)
// {
// 	char	*lzipp;
// 	char	*cgerling;

// 	lzipp = "https://profile.intra.42.fr/users/lzipp";
// 	cgerling = "https://profile.intra.42.fr/users/cgerling";
// 	printf("\033[0;32m\n\n");
// 	printf("	██████╗  █████╗ ██████╗  █████╗ ███████╗██╗  ██╗\n");
// 	printf("	██╔══██╗██╔══██╗██╔══██╗██╔══██╗██╔════╝██║  ██║\n");
// 	printf("	██████╔╝███████║██████╔╝███████║███████╗███████║\n");
// 	printf("	██╔══██╗██╔══██║██╔══██╗██╔══██║╚════██║██╔══██║\n");
// 	printf("	██████╔╝██║  ██║██████╔╝██║  ██║███████║██║  ██║\n");
// 	printf("	╚═════╝ ╚═╝  ╚═╝╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝");
// 	printf("\033[1;33m\n\n");
// 	printf("		   born again born again shell\n\n");
// 	printf("\033[1;36m			");
// 	printf("\e]8;;%s\alzipp\e]8;;\a && ", lzipp);
// 	printf("\e]8;;%s\acgerling\e]8;;\a\n", cgerling);
// 	printf("\033[0m\n");
// }
