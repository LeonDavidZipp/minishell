/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:24:57 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/01 10:38:04 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	init_app_data(t_app_data *app_data, char **envp);
static char	*get_input(t_app_data *app_data);
// static void	print_logo(void);
// static void	print_logo(void);

int	g_exit_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_app_data	app_data;

	(void)argv;
	if (argc != 1)
	{
		// printf("\033[0;31mUsage: ./minishell\033[0m\n");
		return (1);
	}
	init_app_data(&app_data, envp);
	signal_handler();
	// print_logo();
	// print_logo();
	while (true)
	{
		g_exit_signal = 0;
		app_data.input = get_input(&app_data);
		if (app_data.input == NULL)
			continue ;
		lexer(&app_data);
	}
	return (0);
}

static char	*get_input(t_app_data *app_data)
{
	char	*input;

	(void)app_data;
	if (isatty(fileno(stdin)))
		input = readline(PROMPT);
	else
	{
		char *line;
		line = get_next_line(fileno(stdin));
		input = ft_strtrim(line, "\n");
		free(line);
	}
	// input = readline(PROMPT);
	if (input && ft_strlen(input) == 0)
	{
		free(input);
		return (NULL);
	}
	else if (input == NULL)
	{
		free(input);
		builtin_exit(app_data, NULL);
	}
	else if (ft_str_isspaces(input))
	{
		free(input);
		return (NULL);
	}
	add_history(input);
	return (input);
}

static void	init_app_data(t_app_data *app_data, char **envp)
{
	app_data->env_vars = init_envp(envp);
	app_data->last_exit_code = 0;
	app_data->input = NULL;
}

// static void	print_logo(void)
// {
// 	char	*lzipp;
// 	char	*cgerling;
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
