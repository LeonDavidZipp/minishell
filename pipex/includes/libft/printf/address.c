/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   address.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:43:38 by cgerling          #+#    #+#             */
/*   Updated: 2023/12/10 14:00:24 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_count(unsigned long n)
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

static int	ft_convert(unsigned long n, char ch[])
{
	int		i;
	long	temp;

	i = 0;
	while (n > 0)
	{
		temp = n % 16;
		if (temp > 9)
			ch[i++] = temp + 87;
		else
			ch[i++] = temp + 48;
		n = n / 16;
	}
	return (i);
}

int	ft_putaddress(void *ptr)
{
	unsigned long	n;
	int				i;
	int				count;
	int				print;
	char			ch[100];

	i = 0;
	n = (unsigned long)ptr;
	if (ft_putstr("0x") == -1)
		return (-1);
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
	return (count + 2);
}
