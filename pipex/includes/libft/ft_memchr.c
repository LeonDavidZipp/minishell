/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 14:20:43 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/13 16:07:35 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;
	char	*ptr;

	i = 0;
	ptr = (char *) s;
	while (i < n)
	{
		if (ptr[i] == (char) c)
			return (&ptr[i]);
		i++;
	}
	return (0);
}

/*int main()
{
	char	str[] = "Hello";
	char	ch = 'H';
	printf("%p\n", ft_memchr(str, ch, 5));;
	return (0);
}*/