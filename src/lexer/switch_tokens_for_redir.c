/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_tokens_for_redir.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 14:51:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/13 14:53:03 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_token	*switch_first_arg_for_redir(t_token *token);

t_token	*switch_args_for_redir(t_token *token)
{
	t_token			*current;
	t_token			*temp[4];

	token = switch_first_arg_for_redir(token);
	current = token;
	while (current && current->next && current->next->next
		&& current->next->next->next)
	{
		if ((current->next->type >= REDIR_OUT && current->next->type <= HEREDOC)
			&& current->next->next->type == ARG
			&& current->next->next->next->type == ARG)
		{
			temp[0] = current->next;
			temp[1] = current->next->next;
			temp[2] = current->next->next->next;
			temp[3] = current->next->next->next->next;
			current->next = temp[2];
			current->next->next = temp[0];
			current->next->next->next = temp[1];
			current->next->next->next->next = temp[3];
		}
		current = current->next;
	}
	return (token);
}

static t_token	*switch_first_arg_for_redir(t_token *token)
{
	t_token			*current;
	t_token			*temp[4];

	current = token;
	if (current && (current->type >= REDIR_OUT && current->type <= HEREDOC)
		&& current->next && current->next->type == ARG
		&& current->next->next && current->next->next->type == ARG)
	{
		temp[0] = current;
		temp[1] = current->next;
		temp[2] = current->next->next;
		temp[3] = current->next->next->next;
		current = temp[2];
		current->type = CMD;
		current->next = temp[0];
		current->next->next = temp[1];
		current->next->next->next = temp[3];
		token = current;
	}
	return (token);
}
