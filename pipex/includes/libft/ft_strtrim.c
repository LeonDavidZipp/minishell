/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 17:13:49 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/18 18:21:52 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_start(char const *s1, char const *set)
{
	int	start;

	start = 0;
	while (s1[start] != '\0' && ft_strchr(set, s1[start]))
		start++;
	return (start);
}

static int	ft_end(char const *s1, char const *set)
{
	int	end;

	end = ft_strlen(s1) - 1;
	while (end >= 0 && ft_strrchr(set, s1[end]))
		end--;
	return (end);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	int		start;
	int		end;
	int		len;

	if (s1 == NULL || set == NULL)
		return (NULL);
	start = ft_start(s1, set);
	end = ft_end(s1, set);
	len = end - start + 1;
	if (len <= 0)
	{
		str = malloc(1);
		if (str == NULL)
			return (NULL);
		str[0] = '\0';
		return (str);
	}
	str = malloc(len + 1);
	if (str == NULL)
		return (NULL);
	ft_memcpy(str, &s1[start], len);
	str[len] = '\0';
	return (str);
}
/*#include <stdio.h>
int main()
{
	char	str[] = "xxHelloxxx";
	char	str1[] = "x";
	printf("%s\n", ft_strtrim(str, str1));
	return (0);
}*/