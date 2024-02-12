/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:22:58 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/12 17:18:54 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(void)
{
	char	*input;

	signal_handler();
	while (1)
	{
		input = readline("\033[0;36mchl#>	\033[0m");
		if (input == NULL || ft_strncmp(input, "exit", ft_strlen(input)) == 0)
		{
			free(input);
			return (0);
		}
		free(input);
	}
	return (0);
}
