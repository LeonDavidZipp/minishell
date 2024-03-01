/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:56:02 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 15:23:43 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_cd(char *path)
{
	if (!path)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("%s: cd: HOME not set\n", PROMPT);
			return ;
		}
	}
	if (chdir(path) == -1)
	{
		printf("%s: cd: %s: %s\n", PROMPT, path, strerror(errno));
	}
}
