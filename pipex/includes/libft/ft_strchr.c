/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 21:19:46 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/13 18:35:10 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*ptr;

	c = c % 256;
	i = 0;
	ptr = (char *) s;
	while (ptr[i] != '\0')
	{
		if (ptr[i] == c)
			return (&ptr[i]);
		i++;
	}
	if (c == '\0')
		return (&ptr[i]);
	return (0);
}

/*int main()
{
    char str[10] = "Charlotte";
    char ch = 'o';
    printf("%s\n", ft_strchr(str, ch));
    return(0);
}*/