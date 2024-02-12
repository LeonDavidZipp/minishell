/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 09:18:06 by lzipp             #+#    #+#             */
/*   Updated: 2023/12/01 11:03:01 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static void	send_char(int pid, char c)
{
	int	bit;

	bit = 0;
	while (bit < 32)
	{
		if (c & (0x01 << bit))
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		bit++;
		usleep(SLEEP_TIME);
	}
}

static void	send_null(int pid)
{
	int	bit;

	bit = 0;
	while (bit < 32)
	{
		kill(pid, SIGUSR2);
		bit++;
		usleep(SLEEP_TIME);
	}
}

static void	server_acknowledge(int signal)
{
	(void)signal;
	write(1, "server received message\n", 24);
	exit(0);
}

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		write(2, "Error: Wrong number of arguments\n", 33);
		write(2, "Usage: ./client [server PID] [message]\n", 39);
		return (1);
	}
	while (*argv[2])
	{
		send_char(ft_atoi(argv[1]), *argv[2]);
		argv[2]++;
	}
	signal(SIGUSR1, server_acknowledge);
	signal(SIGUSR2, server_acknowledge);
	send_null(ft_atoi(argv[1]));
	while (1)
		pause();
	return (0);
}
