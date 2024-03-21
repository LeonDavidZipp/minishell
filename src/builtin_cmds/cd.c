/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:56:02 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/20 11:49:40 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_cd(char *path)
{
	if (!path)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("%s: cd: HOME not set\n", PROMPT);
			return (1);
		}
	}
	if (chdir(path) == -1)
	{
		printf("%s: cd: %s: %s\n", PROMPT, path, strerror(errno));
		return (1);
	}
	return (0);
}
