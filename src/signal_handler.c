/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 11:58:14 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/12 15:32:06 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void		handle_ctrl_c(int signal);
static void		handle_ctrl_d(int signal);

void	signal_handler(void)
{
	signal(SIGINT, handle_ctrl_c);
	signal(EOF, handle_ctrl_d);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_ctrl_c(int signal)
{
	(void)signal;
	printf("\n\033[0;36mchl #>\033[0m");
}

void	handle_ctrl_d(int signal)
{
	(void)signal;
	exit(0);
}
