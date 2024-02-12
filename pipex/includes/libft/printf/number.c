/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   number.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1012/10/20 11:10:28 by cgerling          #+#    #+#             */
/*   Updated: 2023/12/10 14:00:35 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_count(long nb)
{
	int	count;

	count = 0;
	if (nb == 0)
	{
		if (ft_putchar('0') == -1)
			return (-1);
		return (1);
	}
	if (nb < 0)
	{
		if (write(1, "-", 1) == -1)
			return (-1);
		count++;
		nb = -nb;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		count++;
	}
	return (count);
}

static int	ft_convert(long nb, char ch[])
{
	int		i;
	long	temp;

	i = 0;
	while (nb > 0)
	{
		temp = nb % 10;
		ch[i++] = temp + 48;
		nb = nb / 10;
	}
	return (i);
}

int	ft_putnbr(int n)
{
	long	nb;
	char	ch[100];
	int		i;
	int		count;
	int		numbers;

	nb = n;
	i = 0;
	count = ft_count(nb);
	if (count == -1)
		return (-1);
	if (nb < 0)
		nb = -nb;
	numbers = ft_convert(nb, ch);
	if (numbers == -1)
		return (-1);
	i = numbers - 1;
	while (i >= 0)
	{
		if (ft_putchar(ch[i--]) == -1)
			return (-1);
	}
	return (count);
}

int	ft_putunsigned(unsigned int n)
{
	long	nb;
	char	ch[100];
	int		count;
	int		numbers;
	int		i;

	nb = n;
	i = 0;
	count = ft_count(nb);
	if (count == -1)
		return (-1);
	if (nb < 0)
		nb = -nb;
	numbers = ft_convert(nb, ch);
	if (numbers == -1)
		return (-1);
	i = numbers - 1;
	while (i >= 0)
	{
		if (ft_putchar(ch[i--]) == -1)
			return (-1);
	}
	return (count);
}
