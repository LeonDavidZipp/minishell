/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 14:38:46 by cgerling          #+#    #+#             */
/*   Updated: 2023/12/13 11:53:27 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_word(char const *s, char c)
{
	int		i;
	int		j;
	int		count;

	i = 0;
	j = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			if (j)
				j = 0;
		}
		else
		{
			if (!j)
			{
				count++;
				j = 1;
			}
		}
		i++;
	}
	return (count);
}

void	ft_free(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		i;
	int		j;
	int		str_index;

	str = (char **)malloc(((ft_count_word(s, c)) + 1) * sizeof(char *));
	if (!str || !s)
		return (free(str), NULL);
	i = 0;
	str_index = 0;
	while (str_index < (ft_count_word(s, c)))
	{
		while (s[i] == c)
			i++;
		j = i;
		while (s[j] && s[j] != c)
			j++;
		str[str_index] = ft_substr(s, i, j - i);
		if (!str[str_index])
			return (ft_free(str), NULL);
		i = j;
		str_index++;
	}
	str[str_index] = NULL;
	return (str);
}
