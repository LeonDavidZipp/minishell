/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 11:58:07 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/20 12:27:35 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*temp;

	if (*lst == NULL)
		return ;
	current = *lst;
	while (current->next != NULL)
	{
		temp = current->next;
		(*del)(current->content);
		free(current);
		current = temp;
	}
	(*del)(current->content);
	free(current);
	*lst = NULL;
}
