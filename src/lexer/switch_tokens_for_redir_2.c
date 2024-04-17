/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_tokens_for_redir_2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 14:51:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/18 17:22:33 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	rearrange_condition(t_token *current)
{
	if (current && current->next
		&& current->next->next
		&& current->type >= REDIR_OUT && current->type <= HEREDOC
		&& (current->next->type == ARG || current->next->type == CMD)
		&& (current->next->next->type == ARG
			|| current->next->next->type == CMD))
		return (true);
	return (false);
}

bool	before_first_condition(t_token *current)
{
	if (current && current->next
		&& current->next->type >= REDIR_OUT
		&& current->next->type <= HEREDOC)
		return (true);
	return (false);
}

void	rearrange_first_element(t_token **tokens, t_token **current,
				t_token **before_first)
{
	t_token			*temp[3];

	temp[0] = *tokens;
	temp[1] = (*current)->next->next;
	temp[2] = (*current)->next->next->next;
	*before_first = temp[1];
	(*before_first)->type = CMD;
	(*before_first)->next = temp[0];
	(*current)->next->next = temp[2];
}

void	rearrange_following_element(t_token **current, t_token **before_first)
{
	t_token			*temp[3];

	temp[0] = (*current)->next->next;
	temp[1] = (*before_first)->next;
	temp[2] = (*current)->next->next->next;
	(*before_first)->next = temp[0];
	(*before_first)->next->next = temp[1];
	(*current)->next->next = temp[2];
	*before_first = (*before_first)->next;
}
