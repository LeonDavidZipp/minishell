/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:56:02 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/26 11:06:31 by lzipp            ###   ########.fr       */
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
			ft_fprintf(2, "%s: cd: HOME not set\n", PROMPT);
			return (1);
		}
	}
	if (chdir(path) == -1)
	{
		ft_fprintf(2, "%s: cd: %s: %s\n", PROMPT, path, strerror(errno));
		return (1);
	}
	return (0);
}
