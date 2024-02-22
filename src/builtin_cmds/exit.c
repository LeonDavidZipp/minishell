/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 12:53:27 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/22 12:59:21 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_exit(int exit_code)
{
	// free memory!!!!!!!!!!!!!!!!
	// free memory!!!!!!!!!!!!!!!!
	// free memory!!!!!!!!!!!!!!!!
	printf("exit\n");
	exit(exit_code);
}
