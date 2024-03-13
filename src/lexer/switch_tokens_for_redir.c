/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_tokens_for_redir.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 14:51:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/13 20:13:08 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*switch_tokens_for_redir(t_token *tokens)
{
	t_token			*current;
	t_token			*prev;
	t_token			*before_first;

	current = tokens;
	before_first = NULL;
	while (current)
	{
		if (!before_first)
		{
			// we are before redirect
			if (current->next && current->type >= REDIR_OUT
				&& current->type <= HEREDOC)
			{
				before_first = current;
			}
			// we are on a redirect
			else if (current->type >= REDIR_OUT && current->type <= HEREDOC
					&& current->next && current->next)
			{
			before_first = current->next->next;
			before_first->next = current;
			before_first->next->next = current->next;
			before_first->next->next->next = current->next->next->next;
			tokens = before_first;
			current = before_first->next;
			}
		}
		break ;
	}
	t_token *temp = tokens;
	while (temp)
	{
		printf("content: %s\n", temp->content);
		temp = temp->next;
	}
	return (tokens);
}
