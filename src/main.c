/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:22:58 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/13 14:36:38 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*get_input(void);

int	main(void)
{
	char	*input;

	signal_handler();
	while (1)
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

	input = readline("\033[0;36mchl#>	\033[0m");
	if (input == NULL)
	{
		printf("exit\n");
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
		exit(0);
	}
	add_history(input);
	return (input);
}
