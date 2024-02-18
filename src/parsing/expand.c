/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 10:43:26 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/18 16:10:42 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>

// version to expand before tokenization (probably won't use this)

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

char	*expand_exit_code(char **str, int *j, int last_exit_code)
{
	char	*exit_code;
	int		i;

	exit_code = ft_itoa(last_exit_code);
	if (!exit_code)
		return (NULL);
	i = 0;
	while (exit_code[i])
	{
		(*str)[*j] = exit_code[i];
		(*j)++;
		i++;
	}
	free(exit_code);
	return (*str);
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

char	*get_pattern(char *input, int *i, int *position)
{
	int		start;
	int		end;
	int		tmp;
	char	*pattern;

	tmp = *i;
	start = 0;
	end = 0;
	while (*i > 0 && !is_whitespace(input[*i]))
		(*i)--;
	start = *i + 1;
	*position = tmp - start;
	*i = tmp;
	while (input[*i] && !is_whitespace(input[*i]))
		(*i)++;
	end = *i;
	pattern = ft_strndup(input + start, end - start);
	if (!pattern)
		return (NULL);
	return (pattern);
}

int	env_var_size(char *input, int *i)
{
	int		start;
	int		size;
	char	*name;
	char	*value;

	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	name = ft_strndup(input + start, *i - start);
	if (!name)
		return (0);
	value = getenv(name);
	if (value)
		size = ft_strlen(value);
	else
		size = 0;
	free(name);
	return (size);
}

int	dir_entry_size(char *input, int *i)
{
	char			*pattern;
	int				position;
	DIR				*dir;
	struct dirent	*entry;
	int				size;
	bool			flag;

	flag = false;
	pattern = get_pattern(input, i, &position);
	if (!pattern)
		return (0);
	dir = opendir(".");
	if (dir == NULL)
		return (0);
	entry = readdir(dir);
	size = 0;
	while (entry != NULL)
	{
		if (match(pattern, entry->d_name))
		{
			if (!flag)
			{
				size -= position;
				flag = true;
			}
			size += ft_strlen(entry->d_name) + 1; // maybe + 1 is wrong???
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (size);
}

int	get_new_size(char *input, int last_exit_code)
{
	int	i;
	int	size;
	int	start;

	i = 0;
	size = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '?')
		{
			size += ft_numlen(last_exit_code);
			i += 2;
		}
		else if (input[i] == '$')
		{
			i++;
			size += env_var_size(input, &i);
		}
		else if (input[i] == '*')
			size += dir_entry_size(input, &i);
		else
			size++;
		i++;
	}
	return (size);
}

// expand: remove last space, fix size_function

char	*expand(char *input, char **envp, int last_exit_code)
{
	char	*output;
	bool	s_quotes;
	bool	d_quotes;
	int		i;
	int		j;
	int		start;
	int		size;

	i = 0;
	j = 0;
	int test = ft_strlen(input);
	printf("%d\n", test);
	size = get_new_size(input, last_exit_code);
	printf("%d\n", size);
	s_quotes = false;
	d_quotes = false;
	output = ft_calloc((size), sizeof(char));
	if (!output)
		return (NULL);
	while (input[i])
	{
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
					output[j++] = value[k++];
				}
				free(name);
			}
			else
				free(name);
		}
		else if (input[i] == '*' && !s_quotes && !d_quotes)
		{
			DIR *dir;
			struct dirent *entry;
			char *pattern;
			int	position;
			bool	flag;

			flag = false;
			pattern = get_pattern(input, &i, &position);
			if (!pattern)
				return (free(output), NULL);
			dir = opendir(".");
			if (dir == NULL)
				return (free(output), NULL);
			entry = readdir(dir);
			while (entry != NULL)
			{
				if (match(pattern, entry->d_name))
				{
					if (!flag)
					{
						j -= position;
						flag = true;
					}
					int k = 0;
					while (entry->d_name[k])
						output[j++] = entry->d_name[k++];
					output[j++] = ' ';
				}
				entry = readdir(dir);
			}
			closedir(dir);
		}
		else
			output[j++] = input[i++];
	}
	output[j] = '\0';
	return (output);
}

int main()
{
	char *input = "hello *.c";
	char *output = expand(input, NULL, 1);
	printf("%s\n", output);
	free(output);
	return (0);
}
