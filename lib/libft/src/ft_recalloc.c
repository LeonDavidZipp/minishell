/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_recalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 15:22:54 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/04 00:05:07 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

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
