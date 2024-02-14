/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 16:55:37 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/14 12:28:07 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include <stdio.h>

typedef struct s_split
{
	int		quote_1;
	int		quote_2;
	int		count;
	int		i;
	int		j;
	int		str_index;
	char	**str;
}			t_split;

void	init_struct(t_split *split)
{
	split->quote_1 = 0;
	split->quote_2 = 0;
	split->count = 0;
	split->i = 0;
	split->j = 0;
	split->str_index = 0;
	split->str = NULL;
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	j;
	size_t	s_len;
	char	*res;

	if (!s)
		return (NULL);
	j = 0;
	s_len = ft_strlen(s);
	while (j + start < s_len && s[j + start] && j < len)
		j++;
	res = (char *)malloc((j + 1) * sizeof(char));
	if (!res)
		return (NULL);
	j = 0;
	while (start < s_len && s[start] && len > 0)
	{
		res[j++] = s[start++];
		len--;
	}
	res[j] = '\0';
	return (res);
}

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	count_word(char const *s, char c)
{
	t_split	split;

	init_struct(&split);
	while (s[split.i])
	{
		if (s[split.i] == '\'')
			split.quote_1 = !split.quote_1;
		if (s[split.i] == '"')
			split.quote_2 = !split.quote_2;
		if (s[split.i] == c && !split.quote_1 && !split.quote_2)
		{
			if (split.j)
				split.j = 0;
		}
		else
		{
			if (!split.j)
			{
				split.count++;
				split.j = 1;
			}
		}
		split.i++;
	}
	return (split.count);
}

char	*process_substring(t_split *split, char const *s, char c)
{
	while ((s[split->j] && (s[split->j] != c || split->quote_1
			|| split->quote_2)))
	{
		if (s[split->j] == '\'')
			split->quote_1 = !split->quote_1;
		else if (s[split->j] == '"')
			split->quote_2 = !split->quote_2;
		split->j++;
	}
	return (ft_substr(s, split->i, split->j - split->i));
}

char	**split(char const *s, char c)
{
	t_split	split;

	init_struct(&split);
	split.str = (char **)malloc(((count_word(s, c)) + 1) * sizeof(char *));
	if (!split.str || !s)
		return (free(split.str), NULL);
	while (split.str_index < (count_word(s, c)))
	{
		while (s[split.i] == c && !split.quote_1 && !split.quote_2)
			split.i++;
		split.j = split.i;
		split.str[split.str_index] = process_substring(&split, s, c);
		if (!split.str[split.str_index])
			return (ft_free_tab(split.str), NULL);
		split.i = split.j;
		split.str_index++;
	}
	split.str[split.str_index] = NULL;
	return (split.str);
}

int main()
{
	char **str = split("lala bla 'hallo du' \"hallo du $USER\" cat | ls (ich du)", ' ');
	for (int i = 0; str[i]; i++)
	{
		printf("%s\n", str[i]);
	}
	return 0;
}