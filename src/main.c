/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:22:58 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/12 19:30:16 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

volatile sig_atomic_t	g_ctrl_c_pressed = 0;

int	main(void)
{
	char	*input;

	signal_handler();
	while (1)
	{
		input = readline("\033[0;36mchl#>	\033[0m");
		if (input == NULL)
			return (0);
		else if (ft_strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		else if (ft_strncmp(input, "exit", ft_strlen(input)) == 0)
		{
			free(input);
			return (0);
		}
		if (g_ctrl_c_pressed)
		{
			g_ctrl_c_pressed = 0;
			continue ;
		}
		printf("%s\n", input);
		free(input);
	}
	return (0);
}
