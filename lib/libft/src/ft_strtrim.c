/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 11:33:36 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/05 18:40:18 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	in_set(char c, char const *set)
{
	int	i;

	if (ft_strlen(set) == 0)
		return (0);
	i = -1;
	while (set[++i])
	{
		if (c == set[i])
			return (1);
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		front_i;
	int		rev_i;
	char	*res;

	if (!s1 || !set)
		return (NULL);
	if (ft_strlen(s1) == 0)
		return ((char *)s1);
	front_i = 0;
	rev_i = ft_strlen(s1) - 1;
	while (s1[front_i] && in_set(s1[front_i], set))
		front_i++;
	while (rev_i > 0 && s1[rev_i] && in_set(s1[rev_i], set))
		rev_i--;
	res = (char *)ft_calloc(rev_i + 2 - front_i, sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (front_i <= rev_i)
		res[i++] = s1[front_i++];
	return (res);
}
