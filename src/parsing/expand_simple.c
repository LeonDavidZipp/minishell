/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 14:59:08 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/19 14:00:18 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>

// version to expand after tokenization (probably makes more sense)

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

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\0')
		return (1);
	return (0);
}

char	*ft_strdup(const char *s1)
{
	char	*dest;
	int		i;

	if (!s1)
		return (NULL);
	i = 0;
	while (s1[i])
		i++;
	dest = (char *)malloc((i + 1) * sizeof (char));
	if (!dest)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

bool	match(char *pattern, char *string)
{
	while (*pattern && *string)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			while (*string)
			{
				if (match(pattern, string))
					return (true);
				string++;
			}
			return (match(pattern, string));
		}
		else if (*pattern == *string)
		{
			pattern++;
			string++;
		}
		else
			return (false);
	}
	return (*pattern == *string);
}

char	*expand_exit_code(int last_exit_code)
{
	char	*exit_code;

	exit_code = ft_itoa(last_exit_code);
	if (!exit_code)
		return (NULL);
	return (exit_code);
}

char	*expand_var(char *input, char **envp)
{
	char	*name;
	char	*value;
	char	*output;
	int		i;

	i = 1;
	while (ft_isalnum(input[i]) || input[i] == '_')
		i++;
	name = ft_strndup(input + 1, i - 1);
	if (!name)
		return (NULL);
	value = getenv(name);
	if (value)
	{
		output = ft_strdup(value);
		if (!output)
			return (NULL);
		free(name);
		return (output);
	}
	free(name);
	return (NULL);
}

char	**expand_wildcard(char *input)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**output;
	int				i;

	i = 0;
	output = (char **)ft_calloc(100, sizeof(char *)); // need to change 100
	if (!output)
		return (NULL);
	dir = opendir(".");
	if (dir == NULL)
		return (NULL);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (match(input, entry->d_name))
			output[i++] = ft_strdup(entry->d_name);
		entry = readdir(dir);
	}
	if (i == 0)
		return (free(output), NULL);
	output[i] = NULL;
	closedir(dir);
	return (output);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input = "*";
	int		i = 0;
	if (input[i] == '$' && input[i + 1] == '?')
	{
		char *exit_code = expand_exit_code(0); // 0 for testing
		printf("%s\n", exit_code); // 0 for testing
		free(exit_code);
	}
	else if (input[i] == '$')
	{
		char *output = expand_var(input, envp);
		if (output)
		{
			printf("%s\n", output);
			free(output);
		}
	}
	else
	{
		char **output = expand_wildcard(input);
		int j = 0;
		while (output[j])
		{
			printf("%s\n", output[j]);
			free(output[j]);
			j++;
		}
		free(output);
	}
	return (0);
}