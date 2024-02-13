/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 12:10:27 by cgerling          #+#    #+#             */
/*   Updated: 2023/12/10 14:00:38 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_putchar(char c)
{
	if (write(1, &c, 1) == -1)
		return (-1);
	return (1);
}

int	ft_putstr(char *s)
{
	int	len;

	if (!s)
	{
		if (write(1, "(null)", 6) == -1)
			return (-1);
		return (6);
	}
	len = ft_strlen(s);
	if (len == -1)
		return (-1);
	if (write(1, s, ft_strlen(s)) == -1)
		return (-1);
	return (len);
}
