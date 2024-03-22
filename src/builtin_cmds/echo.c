/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/22 15:19:29 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_echo(char *str, int out_fd)
{
	char	*temp;

	if (!str)
	{
		write(out_fd, "\n", 1);
		return (0);
	}
	if (!str)
	{
		write(out_fd, "\n", 1);
		return (0);
	}
	if (ft_strlen(str) >= 3 && ft_strncmp(str, "-n ", 3) == 0)
	{
		temp = ft_substr(str, 3, ft_strlen(str) - 3);
		ft_putstr_fd(temp, out_fd);
		free(temp);
	}
	else if (ft_strlen(str) == 2 && ft_strncmp(str, "-n", 2) == 0)
	{
		write(out_fd, "\n", 1);
	}
	else
	{
		ft_putstr_fd(str, out_fd);
		write(out_fd, "\n", 1);
	}
	return (0);
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
