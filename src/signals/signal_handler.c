/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 11:58:14 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/22 15:20:33 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void		handle_ctrl_c(int signal);

void	signal_handler(void)
{
	signal(SIGINT, handle_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_ctrl_c(int signal)
{
	char eof = 4;
	(void)signal;
	printf("\n");
	if (g_exit_signal == 0)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_exit_signal == 2)
		ioctl(0, TIOCSTI, &eof);
}
