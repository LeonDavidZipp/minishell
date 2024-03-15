/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_tokens_for_redir.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 14:51:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/15 19:07:29 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	rearrange_at_beginning(t_token **tokens, t_token **current,
				t_token **before_first);
static void	rearrange_first_time(t_token **current, t_token **before_first);
static void	rearrange_tokens(t_token **current, t_token **before_first);

t_token	*switch_tokens_for_redir(t_token *tokens)
{
	// go through all tokens: while loop
	// while there is no redir operator, simply go to next: second while loop
	// 		while (current->next && current->next->type >= REDIR_OUT && current->next->type <= HEREDOC)
	// 			current = current->next;
	// if the next token is a redir
	// 		1. before_first doesnt exist
	// 		before_first is set to current
	// 		2. does exist
	// 		before_first->next is set to curren
	// if ||, && is encountered, before_first = NULL
	t_token			*current;
	t_token			*before_first;

	current = tokens;
	before_first = NULL;
	// check before whether first elem already is before redir
	if (current->next && current->next->type >= REDIR_OUT
		&& current->next->type <= HEREDOC)
		before_first = current;
	else if (current->type >= REDIR_OUT && current->type <= HEREDOC
		&& current->next && current->next->next
		&& (current->next->type == ARG || current->next->type == CMD)
		&& (current->next->next->type == ARG || current->next->next->type == CMD))
		rearrange_at_beginning(&tokens, &current, &before_first);
	// go through all tokens
	while (current)
	{
		// skip over all tokens that dont have redir as token->next
		while (!before_first
			&& current->next
			&& !(current->next->type >= REDIR_OUT && current->next->type <= HEREDOC))
			current = current->next;
		// after skipping, if before first doesnt exist and if token->next is redir, token becomes before_first
		if (!before_first && current && current->next
			&& current->next->type >= REDIR_OUT && current->next->type <= HEREDOC)
			before_first = current;
		// else if it does exist already, insert current as next element
		else if (!before_first && current && current->next && current->next->next
			&& current->type >= REDIR_OUT && current->type <= HEREDOC
			&& (current->next->type == ARG || current->next->type == CMD)
			&& (current->next->next->type == ARG || current->next->next->type == CMD))
		{
			rearrange_first_time(&current, &before_first);
			continue ;
		}
		else if (before_first && current && current->next && current->next->next
			&& current->type >= REDIR_OUT && current->type <= HEREDOC
			&& (current->next->type == ARG || current->next->type == CMD)
			&& (current->next->next->type == ARG || current->next->next->type == CMD))
		{
			rearrange_tokens(&current, &before_first);
			continue ;
		}
		if (current && (current->type == AND || current->type == OR))
			before_first = NULL;
		current = current->next;
	}
	return (tokens);
}

static void	rearrange_at_beginning(t_token **tokens, t_token **current, t_token **before_first)
{
	t_token			*temp[4];

	temp[0] = *current;
	temp[1] = (*current)->next;
	temp[2] = (*current)->next->next;
	temp[3] = (*current)->next->next->next;
	*before_first = temp[2];
	(*before_first)->next = temp[0];
	(*before_first)->next->next = temp[1];
	(*before_first)->next->next->next = temp[3];
	*tokens = *before_first;
	*current = *tokens;
}

static void	rearrange_first_time(t_token **current, t_token **before_first)
{
	t_token			*temp[4];

	temp[0] = *current;
	temp[1] = (*current)->next;
	temp[2] = (*current)->next->next;
	temp[3] = (*current)->next->next->next;
	*before_first = temp[2];
	(*before_first)->next = temp[0];
	(*before_first)->next->next = temp[1];
	(*before_first)->next->next->next = temp[3];
}

static void	rearrange_tokens(t_token **current, t_token **before_first)
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
