/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_tokens_for_redir.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 14:51:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/13 22:14:53 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*switch_args_for_redir(t_token *tokens)
{
	t_token			*current;
	t_token			*temp[5];
	t_token			*before_first;

	current = tokens;
	before_first = NULL;
	while (current)
	{
		// no redirect before yet
		if (!before_first)
		{
			// we are before redirect
			if (current->next && current->next->type >= REDIR_OUT
				&& current->next->type <= HEREDOC)
				before_first = current;
			// we are on a redirect
			if (current->type >= REDIR_OUT && current->type <= HEREDOC
					&& current->next && current->next)
			{
			temp[0] = current;
			temp[1] = current->next;
			temp[2] = current->next->next;
			temp[3] = current->next->next->next;
			before_first = temp[2];
			before_first->next = temp[0];
			before_first->next->next = temp[1];
			before_first->next->next->next = temp[3];
			tokens = before_first;
			current = before_first;
			continue ;
			}
		}
		// now we are after the first redirect
		else
		{
			if (current->type >= REDIR_OUT && current->type <= HEREDOC
				&& current->next && (current->next->type == ARG
					|| current->next->type == CMD)
				&& current->next->next && (current->next->next->type == ARG
					|| current->next->next->type == CMD))
			{
				temp[0] = current;
				temp[1] = current->next;
				temp[2] = current->next->next;
				temp[3] = current->next->next->next;
				before_first->next = temp[2];
				before_first->next->next = temp[0];
				before_first->next->next->next = temp[1];
				before_first->next->next->next->next = temp[3];
				before_first = before_first->next;
				current = before_first->next;
				continue ;
			}
		}
		current = current->next;
	}
	return (tokens);
}
