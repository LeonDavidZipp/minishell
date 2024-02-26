/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/26 12:17:28 by intra            ###   ########.fr       */
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
