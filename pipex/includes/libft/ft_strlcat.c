/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 14:54:55 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/14 14:38:57 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	k = 0;
	while (dst[i] != '\0')
		i++;
	while (src[j] != '\0')
		j++;
	if (dstsize <= i)
		j = j + dstsize;
	else
		j = j + i;
	while (src[k] != '\0' && i + 1 < dstsize)
	{
		dst[i++] = src[k++];
	}
	dst[i] = '\0';
	return (j);
}
/*int main()
{
	char	str[] = "Hello ";
	char	str1[] = "World!";
	printf("%zu\n", ft_strlcat(str, str1, 14));
	return (0);
}*/