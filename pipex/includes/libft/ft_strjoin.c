/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:58:48 by cgerling          #+#    #+#             */
/*   Updated: 2023/12/10 13:32:45 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int			len;
	char		*ptr;
	size_t		i;
	size_t		j;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	ptr = malloc(len + 1);
	if (ptr == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[j] != '\0')
		ptr[i++] = s1[j++];
	j = 0;
	while (s2[j] != '\0')
		ptr[i++] = s2[j++];
	ptr[i] = '\0';
	return (ptr);
}
/*#include <stdio.h>
int main()
{
	char	str[] = "Hello ";
	char	str1[] = "World";
	printf("%s\n", ft_strjoin(str, str1));
	return (0);
}*/