/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/25 21:45:05 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_echo(char *str)
{
	char	*temp;

	if (ft_strncmp(str, "-n ", 3) == 0)
	{
		temp = ft_substr(str, 3, ft_strlen(str) - 3);
		printf("%s", temp);
		free(temp);
	}
	else
	{
		printf("%s\n", str);
	}
}
