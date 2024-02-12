/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:25:27 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/04 00:13:23 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int		sep_inside(char *str);
int		catch_errors(int nbt, char **text);
int		file_text(char **text, char *buffer);
char	*next_line(char **text);

char	*get_next_line(int fd)
{
	static char		*text[OPEN_MAX] = {0};
	char			buffer[BUFFER_SIZE + 1];
	ssize_t			nbt;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free(text[fd]), NULL);
	nbt = 1;
	while (nbt > 0)
	{
		ft_bzero(buffer, BUFFER_SIZE + 1);
		nbt = read(fd, buffer, BUFFER_SIZE);
		if (catch_errors(nbt, &text[fd]) == 1)
			return (NULL);
		if (file_text(&text[fd], buffer) == 0)
			return (NULL);
		if (sep_inside(text[fd]) == 1 || (nbt == 0 && text[fd]))
			break ;
	}
	return (next_line(&text[fd]));
}

int	sep_inside(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i++] == '\n')
			return (1);
	}
	return (0);
}

int	catch_errors(int nbt, char **text)
{
	if (nbt < 0 || (nbt == 0 && !(*text)) || (nbt == 0 && (*text)[0] == '\0'))
	{
		if (*text)
			free(*text);
		*text = NULL;
		return (1);
	}
	return (0);
}

int	file_text(char **text, char *buffer)
{
	char	*temp_buffer;

	temp_buffer = NULL;
	if (*text)
	{
		temp_buffer = ft_strjoin(*text, buffer);
		free(*text);
		*text = NULL;
		if (!temp_buffer)
			return (0);
		*text = ft_substr(temp_buffer, 0, ft_strlen(temp_buffer) + 1);
		free(temp_buffer);
	}
	else
		*text = ft_substr(buffer, 0, ft_strlen(buffer) + 1);
	return (1);
}

char	*next_line(char **text)
{
	char			*line;
	char			*temp;
	ssize_t			len;

	len = 0;
	while (*text && (*text)[len])
	{
		if ((*text)[len++] == '\n')
			break ;
	}
	line = ft_substr(*text, 0, len);
	temp = ft_substr(*text, len, ft_strlen(*text) - len);
	free(*text);
	(*text) = NULL;
	if (!line || !temp)
	{
		free(line);
		free(temp);
		return (NULL);
	}
	(*text) = ft_substr(temp, 0, ft_strlen(temp) + 1);
	free(temp);
	if (!(*text))
		return (free(line), NULL);
	return (line);
}
