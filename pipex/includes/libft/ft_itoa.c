/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:22:54 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/17 15:16:10 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_mem_alloc(int size)
{
	char	*str;

	str = malloc(size + 1);
	if (str == NULL)
		return (NULL);
	str[size] = '\0';
	return (str);
}

static char	*ft_zero(void)
{
	char	*str;

	str = ft_mem_alloc(1);
	if (str != NULL)
		str[0] = '0';
	return (str);
}

static int	ft_size_calc(long nb)
{
	int	size;

	size = 0;
	while (nb > 0)
	{
		size++;
		nb /= 10;
	}
	return (size);
}

static void	ft_convert(long nb, char *str, int size)
{
	int	i;

	i = size - 1;
	while (nb > 0)
	{
		str[i] = 48 + (nb % 10);
		nb /= 10;
		i--;
	}
}

char	*ft_itoa(int n)
{
	int		size;
	int		neg;
	long	nb;
	char	*str;

	if (n == 0)
		return (ft_zero());
	size = 0;
	neg = 0;
	nb = (long)n;
	if (n < 0)
	{
		size++;
		neg = 1;
		nb = -nb;
	}
	size += ft_size_calc(nb);
	str = ft_mem_alloc(size);
	if (str == NULL)
		return (NULL);
	ft_convert(nb, str, size);
	if (neg)
		str[0] = '-';
	return (str);
}
/*#include <stdio.h>
int main()
{
	printf("%s\n", ft_itoa(-2147483648));
	return (0);
}*/
