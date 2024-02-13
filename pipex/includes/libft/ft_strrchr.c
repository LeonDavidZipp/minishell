/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 19:33:07 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/13 16:10:14 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*ptr;
	char	*ptr2;

	i = 0;
	ptr = (char *) s;
	while (ptr[i] != '\0')
		i++;
	ptr2 = ptr + i;
	while (i >= 0)
	{
		if (*ptr2 == (char) c)
			return (ptr2);
		i--;
		ptr2--;
	}
	return (0);
}

/*int main()
{
    char    str[10] = "Charlotte";
    char    ch = 'l';
    printf("%s\n", ft_strrchr(str, ch));
    return (0);
}*/