/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 15:49:16 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_echo(char *str)
{
	char	*temp;

	if (ft_strlen(str) >= 3 && ft_strncmp(str, "-n ", 3) == 0)
	{
		temp = ft_substr(str, 3, ft_strlen(str) - 3);
		printf("%s", temp);
		free(temp);
	}
	else if (ft_strlen(str) == 2 && ft_strncmp(str, "-n", 2) == 0)
		printf("\n");
	else
		printf("%s\n", str);
}

// int	main(int argc, char **argv)
// {
// 	// int	i;

// 	// i = 1;
// 	if (argc == 1)
// 		printf("\n");
// 	else
// 		builtin_echo(argv[2]);
// 	return (0);
// }
