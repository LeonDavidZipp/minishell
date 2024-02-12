/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 17:12:06 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/13 16:06:03 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*ptrd;
	char	*ptrs;

	i = 0;
	ptrd = dst;
	ptrs = (char *) src;
	if (!dst && !src)
	{
		return (0);
	}
	while (i < n)
	{
		ptrd[i] = ptrs[i];
		i++;
	}
	return (ptrd);
}

/*int main()
{
    char str[9] = "Charlotte";
    char str1[9] = "hallo";
    printf("%s\n", ft_memcpy(str1, str, 6));
    return(0);
}*/
