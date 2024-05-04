/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:54:39 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/14 14:40:19 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_unset(char *keys, char **env_vars)
{
	int			exit_code;

	exit_code = 0;
	if (!keys)
		return (exit_code);
	if (ft_strcmp(keys, "") == 0)
		return (ft_fprintf(2, "%s: unset: `""': %s\n", NAME, INVALID_ID), 1);
	exit_code = unset_multiple_env_vars(keys, &env_vars);
	return (exit_code);
}
