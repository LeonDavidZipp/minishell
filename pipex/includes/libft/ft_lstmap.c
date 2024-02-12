/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 12:48:54 by cgerling          #+#    #+#             */
/*   Updated: 2023/10/20 16:07:36 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*temp;
	t_list	*new;
	t_list	*new_lst;

	if (lst == NULL)
		return (NULL);
	new_lst = NULL;
	while (lst != NULL)
	{
		temp = (*f)(lst->content);
		new = ft_lstnew(temp);
		if (new == NULL)
		{
			free(temp);
			ft_lstclear(&new_lst, *del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, new);
		lst = lst->next;
	}
	return (new_lst);
}
