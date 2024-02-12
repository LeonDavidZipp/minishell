/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchars_int.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:49:52 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/04 00:05:07 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_putchar_int(const char c)
{
	return (write(1, &c, 1));
}

int	ft_putstr_int(const char *s)
{
	int	len;

	len = 0;
	while (s[len])
	{
		if (write(1, &s[len], 1) == -1)
			return (-1);
		len++;
	}
	return (len);
}
