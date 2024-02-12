/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 14:06:54 by leon              #+#    #+#             */
/*   Updated: 2024/02/04 00:19:39 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_convert(va_list *args, char c)
{
	char	*s;

	if (c == 'c')
		return (ft_putchar_int(va_arg(*args, int)));
	else if (c == 's')
	{
		s = va_arg(*args, char *);
		if (s == NULL)
			s = "(null)";
		return (ft_putstr_int(s));
	}
	else if (c == 'd' || c == 'i')
		return (ft_putnbr_int(va_arg(*args, int), 0));
	else if (c == 'u')
		return (ft_putu_int(va_arg(*args, unsigned int), 0));
	else if (c == 'x')
		return (ft_puthex_small_int(va_arg(*args, unsigned int), 0));
	else if (c == 'X')
		return (ft_puthex_big_int(va_arg(*args, unsigned int), 0));
	else if (c == 'p')
		return (ft_putpointer_int(va_arg(*args, unsigned long), 0));
	else if (c == '%')
		return (ft_putchar_int('%'));
	else
		return (-1);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		i;
	int		len;
	int		worked;

	i = 0;
	len = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] != '%')
			worked = ft_putchar_int(str[i++]);
		else
		{
			worked = ft_convert(&args, str[i + 1]);
			i += 2;
		}
		if (worked == -1)
			return (-1);
		len += worked;
	}
	va_end(args);
	return (len);
}

int	ft_printfn(const char *str, ...)
{
	va_list	args;
	int		i;
	int		len;
	int		worked;

	i = 0;
	len = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] != '%')
			worked = ft_putchar_int(str[i++]);
		else
		{
			worked = ft_convert(&args, str[i + 1]);
			i += 2;
		}
		if (worked == -1)
			return (-1);
		len += worked;
	}
	va_end(args);
	write(1, "\n", 1);
	return (len + 1);
}
