/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 14:40:57 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/17 15:19:24 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	total;
	size_t	i;
	void	*str;

	total = count * size;
	str = malloc(total);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < total)
	{
		((char *)str)[i] = 0;
		i++;
	}
	return (str);
}
/*int main()
{
	printf("%p\n", ft_calloc(3, 6));
	return (0);
}*/