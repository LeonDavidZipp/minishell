/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 18:07:20 by cgerling          #+#    #+#             */
/*   Updated: 2023/12/20 10:41:09 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static void	ft_free(char **arg1, char **arg2, char	**arg3)
{
	if (arg1 && *arg1)
	{
		free(*arg1);
		*arg1 = NULL;
	}
	if (arg2 && *arg2)
	{
		free(*arg2);
		*arg2 = NULL;
	}
	if (arg3 && *arg3)
	{
		free(*arg3);
		*arg3 = NULL;
	}
}

static int	read_from_fd(int fd, char **buffer, char *temp)
{
	char	*old_buffer;
	ssize_t	bytes_read;

	bytes_read = read(fd, temp, BUFFER_SIZE);
	if (bytes_read == -1)
		return (ft_free(buffer, NULL, NULL), -1);
	temp[bytes_read] = '\0';
	if (*buffer == NULL)
		*buffer = ft_strdup("\0");
	if (!*buffer)
		return (-1);
	old_buffer = *buffer;
	*buffer = ft_strjoin(old_buffer, temp);
	if (!*buffer)
		return (ft_free(&old_buffer, buffer, NULL), -1);
	ft_free(&old_buffer, NULL, NULL);
	return (bytes_read);
}

static char	*get_line(char **buffer)
{
	char	*line;
	size_t	i;

	i = 0;
	if (*buffer && *buffer[0] == '\0')
		return (ft_free(buffer, NULL, NULL), NULL);
	while ((*buffer)[i] && (*buffer)[i] != '\n')
		i++;
	if ((*buffer)[i] == '\n')
		i++;
	line = ft_substr(*buffer, 0, i);
	if (!line)
		return (NULL);
	return (line);
}

static char	*update_buffer(char	*buffer)
{
	char	*temp;
	size_t	i;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	temp = ft_substr(buffer, i, ft_strlen(buffer) - i);
	if (!temp)
		return (ft_free(&buffer, NULL, NULL), NULL);
	ft_free(&buffer, NULL, NULL);
	return (temp);
}

char	*get_next_line(int fd)
{
	static char	*buffer = NULL;
	char		*temp;
	char		*line;
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (ft_free(&buffer, NULL, NULL), NULL);
	bytes_read = 1;
	temp = malloc(BUFFER_SIZE + 1);
	if (!temp)
		return (ft_free(&buffer, NULL, NULL), NULL);
	while (bytes_read != 0)
	{
		bytes_read = read_from_fd(fd, &buffer, temp);
		if (bytes_read == -1 || (bytes_read == 0 && !buffer))
			return (ft_free(&buffer, &temp, NULL), NULL);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	line = get_line(&buffer);
	if (!line)
		return (ft_free(&buffer, &temp, NULL), NULL);
	ft_free(&temp, NULL, NULL);
	buffer = update_buffer(buffer);
	return (line);
}

/*#include <fcntl.h>
#include <stdio.h>

int main(void)
{
	int fd;
	char *line;
	
	fd = open("file.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("Error opening file");
		return 1;
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s\n", line);
		free(line);
	}
	close(fd);
	return (0);
}*/
