/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 12:31:07 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/13 15:59:32 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*ptr1;
	unsigned char	*ptr2;

	i = 0;
	ptr1 = (unsigned char *)s1;
	ptr2 = (unsigned char *)s2;
	while (i < n && ptr1[i] != '\0' && ptr2[i] != '\0')
	{
		if (ptr1[i] != ptr2[i])
			return (ptr1[i] - ptr2[i]);
		i++;
	}
	if (i == n)
		return (0);
	return (ptr1[i] - ptr2[i]);
}

/*int main()
{
	char	str[] = "abcdef";
	char	str1[] = "abc\375xx";
	printf("%d\n", ft_strncmp(str, str1, 5));
	return (0);
}*/