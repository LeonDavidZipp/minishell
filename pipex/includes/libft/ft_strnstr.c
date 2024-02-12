/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 16:00:56 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/13 15:57:45 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (*needle == 0)
		return ((char *) haystack);
	while (haystack[i] != '\0' && i < len)
	{
		if (haystack[i++] == needle[j++])
		{
			if (needle[j] == '\0')
				return (&((char *)haystack)[i - j]);
		}
		else
		{
			if (j == 0)
				i++;
			else
				i = i - j + 1;
			j = 0;
		}
	}
	return (0);
}
/*int main()
{
	char	str[] = "aaabcabcd";
	char	str1[] = "abcd";
	printf("%s\n", ft_strnstr(str, str1, 9));
	return (0);
}*/