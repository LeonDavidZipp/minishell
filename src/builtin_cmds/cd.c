/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:56:02 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/03 17:12:31 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// implement cd ~ and cd -

int	builtin_cd(char *path)
{
	if (!path)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_fprintf(2, "%s: cd: HOME not set\n", NAME);
			return (1);
		}
	}
	if (chdir(path) == -1)
	{
		ft_fprintf(2, "%s: cd: %s: %s\n", NAME, path, strerror(errno));
		return (1);
	}
	return (0);
}
