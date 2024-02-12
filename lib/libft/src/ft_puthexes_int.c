/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthexes_int.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:22:30 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/04 00:05:07 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_puthex_big_int(unsigned int n, int len)
{
	int	worked;

	if (n / 16 > 0)
		len = ft_puthex_big_int(n / 16, len);
	if (len == -1)
		return (-1);
	worked = write(1, &"0123456789ABCDEF"[n % 16], 1);
	if (worked == -1)
		return (-1);
	return (len + worked);
}

int	ft_puthex_small_int(unsigned int n, int len)
{
	int	worked;

	if (n / 16 > 0)
		len = ft_puthex_small_int(n / 16, len);
	if (len == -1)
		return (-1);
	worked = write(1, &"0123456789abcdef"[n % 16], 1);
	if (worked == -1)
		return (-1);
	return (len + worked);
}

int	ft_putpointer_int(unsigned long n, int len)
{
	int	worked;

	if (n / 16 > 0)
		len = ft_putpointer_int(n / 16, len);
	if (len == -1)
		return (-1);
	if (n / 16 == 0)
	{
		worked = write(1, "0x", 2);
		if (worked == -1)
			return (-1);
		len += 2;
	}
	worked = write(1, &"0123456789abcdef"[n % 16], 1);
	if (worked == -1)
		return (-1);
	return (len + worked);
}
