/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_after_cd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:33:38 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/20 14:57:41 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*remove_after_cd(t_token *tokens)
{
	t_token		*current;
	t_token		*temp;

	current = tokens;
	while (current)
	{
		if (current->next && current->next->next
			&& ft_strcmp(current->content, "cd") == 0
			&& current->next->type == ARG
			&& current->next->next->type == ARG)
		{
			temp = current->next->next->next;
			free(current->next->next->content);
			free(current->next->next);
			current->next->next = temp;
			continue ;
		}
		current = current->next;
	}
	return (tokens);
}
