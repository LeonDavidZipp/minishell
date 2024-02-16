/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:56:02 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/14 16:16:50 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_cd(char *path)
{
	if (chdir(path) == -1)
	{
		printf("%s: cd: %s: %s\n", PROMPT, path, strerror(errno));
	}
}
