/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_no_nl.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 00:36:52 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/04 00:38:58 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*get_next_line_no_nl(int fd)
{
	char	*line;
	char	*trimmed_line;

	line = get_next_line(fd);
	trimmed_line = ft_strtrim(line, "\n\t\v\f\r ");
	ft_safe_free((void **)&line);
	return (trimmed_line);
}
