/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hexa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 12:13:28 by cgerling          #+#    #+#             */
/*   Updated: 2023/12/10 14:00:32 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_count(unsigned int n)
{
	int	count;

	count = 0;
	if (n == 0)
	{
		if (ft_putchar('0') == -1)
			return (-1);
		return (1);
	}
	while (n > 0)
	{
		n = n / 16;
		count++;
	}
	return (count);
}

static int	ft_convert(unsigned int n, char ch[])
{
	int		i;
	long	temp;

	i = 0;
	while (n > 0)
	{
		temp = n % 16;
		if (temp > 9)
			ch[i++] = temp + 55;
		else
			ch[i++] = temp + 48;
		n = n / 16;
	}
	return (i);
}

int	ft_puthexa_lo(unsigned int n)
{
	int		i;
	int		count;
	int		print;
	char	ch[100];

	i = 0;
	count = ft_count(n);
	if (count == -1)
		return (-1);
	print = ft_convert(n, ch);
	if (print == -1)
		return (-1);
	i = print - 1;
	while (i >= 0)
	{
		ch[i] = ft_tolower(ch[i]);
		if (ft_putchar(ch[i--]) == -1)
			return (-1);
	}
	return (count);
}

int	ft_puthexa_up(unsigned int n)
{
	int		i;
	int		count;
	int		print;
	char	ch[100];

	i = 0;
	count = ft_count(n);
	if (count == -1)
		return (-1);
	print = ft_convert(n, ch);
	if (print == -1)
		return (-1);
	i = print - 1;
	while (i >= 0)
	{
		if (ft_putchar(ch[i--]) == -1)
			return (-1);
	}
	return (count);
}
