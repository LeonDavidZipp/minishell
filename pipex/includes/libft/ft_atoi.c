/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 22:04:44 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/13 16:08:47 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int		i;
	int		res;
	char	*ptr;

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

/*int main()
{
	char	str[] = "\v \t-42h123";
	printf("%d\n", ft_atoi(str));
	return (0);
}*/