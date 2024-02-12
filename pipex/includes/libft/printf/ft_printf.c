/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 16:43:58 by cgerling          #+#    #+#             */
/*   Updated: 2023/12/10 14:00:29 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_format(va_list args, const char *specifier)
{
	void	*ptr;

	if (*specifier == '%')
		return (ft_putchar('%'));
	else if (*specifier == 'c')
		return (ft_putchar(va_arg(args, int)));
	else if (*specifier == 's')
		return (ft_putstr(va_arg(args, char *)));
	else if (*specifier == 'd' || *specifier == 'i')
		return (ft_putnbr(va_arg(args, int)));
	else if (*specifier == 'u')
		return (ft_putunsigned(va_arg(args, unsigned int)));
	else if (*specifier == 'x')
		return (ft_puthexa_lo(va_arg(args, int)));
	else if (*specifier == 'X')
		return (ft_puthexa_up(va_arg(args, int)));
	else if (*specifier == 'p')
	{
		ptr = va_arg(args, void *);
		return (ft_putaddress(ptr));
	}
	else
		return (-1);
}

int	ft_printf(const char *str, ...)
{
	int		len;
	int		i;
	va_list	args;

	len = 0;
	i = 0;
	va_start(args, str);
	while (str[i] != '\0')
	{
		if (str[i] == '%')
		{
			len += ft_format(args, (str + ++i));
			if (len == -1)
				return (-1);
		}
		else
		{
			len += ft_putchar(str[i]);
			if (len == -1)
				return (-1);
		}
		i++;
	}
	va_end(args);
	return (len);
}
