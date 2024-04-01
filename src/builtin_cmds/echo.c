/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/01 11:57:59 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_echo(char *args, int out_fd)
{
	char	*temp;

	if (!args)
	{
		ft_fprintf(out_fd, "\n");
		return (0);
	}
	if (ft_strlen(args) >= 3 && ft_strncmp(args, "-n ", 3) == 0)
	{
		temp = ft_substr(args, 3, ft_strlen(args) - 3);
		ft_putstr_fd(temp, out_fd);
		free(temp);
	}
	else if (ft_strlen(args) == 2 && ft_strncmp(args, "-n", 2) == 0)
		ft_fprintf(out_fd, "\n");
	else
		ft_fprintf(out_fd, "%s\n", args);
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
