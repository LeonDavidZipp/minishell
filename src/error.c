/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 13:17:15 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/20 13:26:36 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	error_free(char *error, bool errno_flag, void *arg1, void *arg2)
{
	if (arg1)
		free(arg1);
	if (arg2)
		free(arg2);
	if (errno_flag)
		perror(error);
	else
		ft_putstr_fd(error, 2);
}
