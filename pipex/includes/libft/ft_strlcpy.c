/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 16:31:19 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/13 16:02:38 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	len;
	char	*ptrd;
	char	*ptrs;

	i = 0;
	len = 0;
	ptrd = dst;
	ptrs = (char *) src;
	if (dstsize > 0)
	{
		while (ptrs[i] != '\0' && i < dstsize - 1)
		{
			ptrd[i] = ptrs[i];
			i++;
		}
		ptrd[i] = '\0';
	}
	while (ptrs[len] != '\0')
		len++;
	return (len);
}

/*int main()
{
	char str[6] = "Hello";
	char str1[20];
	ft_strlcpy(str1, str, sizeof(str1));
	printf("%s\n", str1);
	return (0);
}*/