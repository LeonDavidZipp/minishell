/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:43:08 by cgerling          #+#    #+#             */
/*   Updated: 2023/12/07 19:37:01 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long	ft_atoll(const char *str)
{
	int			i;
	long long	res;
	char		*ptr;

	i = 0;
	res = 0;
	ptr = (char *) str;
	while ((*ptr >= 9 && *ptr <= 13) || *ptr == 32)
		ptr++;
	if (*ptr == 45)
	{
		i++;
		ptr++;
	}
	else if (*ptr == 43)
		ptr++;
	while (*ptr >= 48 && *ptr <= 57)
	{
		res = res * 10;
		res = res + *ptr - 48;
		ptr++;
	}
	if (i == 1)
		res = -res;
	return (res);
}
