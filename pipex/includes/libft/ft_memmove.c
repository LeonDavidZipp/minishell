/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 15:12:51 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/13 16:05:03 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;
	char	*ptrs;

	i = 0;
	ptrs = (char *) src;
	if (!dst && !src)
		return (0);
	if ((char *)dst < ptrs)
	{
		while (i < len)
		{
			((char *)dst)[i] = ptrs[i];
			i++;
		}
	}
	else
	{
		while (len > 0)
		{
			((char *)dst)[len - 1] = ptrs[len - 1];
			len--;
		}
	}
	return (dst);
}

/*int	main()
{
    char str[25] = "hello world";
	char str1[25] = "World";
    printf("%s\n", ft_memmove(str1, str, 11));
    return (0);
}*/