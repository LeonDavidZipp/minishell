/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 10:43:26 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/15 17:40:56 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

static int	ft_numlen(long nb)
{
	int	len;

	len = 0;
	if (nb == 0)
		return (1);
	else if (nb < 0)
	{
		nb *= -1;
		len++;
	}
	while (nb > 0)
	{
		len++;
		nb /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	long	nb;
	int		len;
	char	*result;
	int		start;

	nb = (long) n;
	len = ft_numlen(nb) + 1;
	start = (nb < 0);
	if (nb < 0)
		nb *= -1;
	result = (char *)malloc(len * sizeof(char));
	if (!result)
		return (NULL);
	if (n < 0)
		result[0] = '-';
	result[len - 1] = '\0';
	len -= 2;
	while (len >= start)
	{
		result[len--] = (nb % 10) + '0';
		nb /= 10;
	}
	return (result);
}

int	ft_isalnum(int c)
{
	if (!(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')
			|| ('0' <= c && '9' >= c)))
		return (0);
	return (1);
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*dest;
	unsigned char	*source;
	size_t			i;

	dest = (unsigned char *)dst;
	source = (unsigned char *)src;
	if (dst > src)
	{
		while (len-- > 0)
			dest[len] = source[len];
	}
	else if (dst < src)
	{
		i = 0;
		while (i < len)
		{
			dest[i] = source[i];
			i++;
		}
	}
	return ((void *)dst);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;
	char	*cptr;
	size_t	total;

	total = count * size;
	if ((count != 0 && total / count != size)
		|| (size != 0 && total / size != count))
		return (NULL);
	ptr = (void *)malloc(total);
	if (!ptr)
		return (NULL);
	cptr = (char *)ptr;
	while (total > 0)
	{
		*cptr = 0;
		cptr++;
		total--;
	}
	return (ptr);
}

void	*ft_recalloc(void *ptr, size_t len, size_t data_size)
{
	void	*new_ptr;

	if (!ptr)
		return (ft_calloc(len, data_size));
	if (len == 0 && ptr)
	{
		free(ptr);
		return (malloc(0));
	}
	new_ptr = ft_calloc(len + 1, data_size);
	if (!new_ptr)
		return (NULL);
	ft_memmove(new_ptr, ptr, len * data_size);
	free(ptr);
	return (new_ptr);
}

char	*expand_exit_code(char **str, int *j, int last_exit_code)
{
	char	*exit_code;
	char	*tmp;
	int		i;

	exit_code = ft_itoa(last_exit_code);
	if (!exit_code)
		return (NULL);
	i = 0;
	while (exit_code[i])
	{
		if (*j == ft_strlen(*str))
		{
			tmp = ft_recalloc(*str, ft_strlen(*str) * 2, sizeof(char));
			if (!tmp)
				return (free(exit_code), NULL);
			*str = tmp;
		}
		(*str)[*j] = exit_code[i];
		(*j)++;
		i++;
	}
	free(exit_code);
	return (*str);
}

char	*ft_strndup(const char *s1, size_t n)
{
	char	*dest;
	size_t	i;

	if (!s1)
		return (NULL);
	i = 0;
	while (s1[i] && i < n)
		i++;
	dest = (char *)malloc((i + 1) * sizeof (char));
	if (!dest)
		return (NULL);
	i = 0;
	while (s1[i] && i < n)
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}


char	*expand(char *input, char **envp, int last_exit_code)
{
	char	*output;
	char	*new_output;
	bool	s_quotes;
	bool	d_quotes;
	int		i;
	int		j;
	int		start;
	int		size;

	i = 0;
	j = 0;
	size = ft_strlen(input) + 1;
	s_quotes = false;
	d_quotes = false;
	output = malloc(sizeof(char) * size);
	if (!output)
		return (NULL);
	while (input[i])
	{
		if (j == size - 1)
		{
			size *= 2;
			new_output = ft_recalloc(output, size, sizeof(char));
			if (!new_output)
				return (free(output), NULL);
			output = new_output;
		}
		if (input[i] == '\'')
			s_quotes = !s_quotes;
		if (input[i] == '\"')
			d_quotes = !d_quotes;
		if (input[i] == '$' && input[i + 1] == '?' && !s_quotes)
		{
			expand_exit_code(&output, &j, last_exit_code);
			i += 2;
		}
		else if (input[i] == '$' && !s_quotes)
		{
			i++;
			start = i;
			while (ft_isalnum(input[i]) || input[i] == '_')
				i++;
			char *name = ft_strndup(input + start, i - start);
			char *value = getenv(name);
			if (value)
			{
				int k = 0;
				while (value[k])
				{
					if (j == size - 1)
					{
						size *= 2;
						new_output = ft_recalloc(output, size, sizeof(char));
						if (!new_output)
							return (free(output), free(name), NULL);
						output = new_output;
					}
					output[j++] = value[k++];
				}
				free(name);
			}
			else
				free(name);
		}
		else if (input[i] == '*' && !s_quotes && !d_quotes)
		{
			// TODO: expand wildcard
		}
		else
			output[j++] = input[i++];
	}
	output[j] = '\0';
	return (output);
}


int main()
{
	char *input = "*.c";
	char *output = expand(input, NULL, 1);
	printf("%s\n", output);
	free(output);
	return (0);
}
