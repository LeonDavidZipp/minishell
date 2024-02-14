/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/14 16:22:08 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_echo(char *str, bool flag_was_set)
{
	printf("%s", str);
	if (flag_was_set)
		printf("\n");
}

int	main(int argc, char **argv)
{
	(void)argc;
	builtin_echo(argv[1], true);
	builtin_echo(argv[1], false);
	return (0);
}
