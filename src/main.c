/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:22:58 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/13 11:38:34 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	main(void)
{
	char	*input;

	signal_handler();
	while (1)
	{
		input = readline("\033[0;36mchl#>	\033[0m");
		if (input == NULL || ft_strncmp(input, "exit", ft_strlen(input)) == 0)
		{
			if (input != NULL)
			{
				add_history(input);
				free(input);
			}
			printf("exit\n");
			return (0);
		}
		else if (ft_strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		add_history(input);
		free(input);
	}
	return (0);
}
