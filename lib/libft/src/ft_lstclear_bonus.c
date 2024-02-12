/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 11:30:36 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/04 00:05:07 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*successor;

	while (*lst != NULL)
	{
		successor = (*lst)->next;
		del((*lst)->content);
		free(*lst);
		*lst = successor;
	}
	free(*lst);
	*lst = NULL;
}
