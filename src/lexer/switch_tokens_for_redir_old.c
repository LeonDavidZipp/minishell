/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_tokens_for_redir_old.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 14:51:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/16 14:07:10 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	rearrange_first_element(t_token **tokens, t_token **current,
				t_token **before_first);
// static void	rearrange_first_time(t_token **current, t_token **before_first);
static void	rearrange_tokens(t_token **current, t_token **before_first);

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

static void	check_and_rearrange_at_beginning(t_token **tokens, t_token **current,
		t_token **before_first)
{
	t_token			*temp[4];

	if ((*current)->next && (*current)->next->type >= REDIR_OUT
		&& (*current)->next->type <= HEREDOC)
		*before_first = *current;
	else if ((*current)->type >= REDIR_OUT && (*current)->type <= HEREDOC
		&& (*current)->next && (*current)->next->next
		&& ((*current)->next->type == ARG || (*current)->next->type == CMD)
		&& ((*current)->next->next->type == ARG
			|| (*current)->next->next->type == CMD))
	{
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
}

t_token	*switch_tokens_for_redir(t_token *tokens)
{
	t_token			*current;
	t_token			*before_first;

	current = tokens;
	before_first = NULL;
	check_and_rearrange_at_beginning(&tokens, &current, &before_first);
	while (current)
	{
		while ((!before_first && !(current->type >= REDIR_OUT && current->type <= HEREDOC) && current->next && !(current->next->type >= REDIR_OUT && current->next->type <= HEREDOC)))
			current = current->next;
		printf("1. currently at node, next: %s, %s\n", current->content, current->next->content);
		while (current && current->next && current->next->next
			&& ((current->type >= REDIR_OUT && current->type <= HEREDOC && (current->next->type == CMD || current->next->type == ARG)
				&& current->next->next->type != CMD && current->next->next->type != ARG)
				)
				)
		{
			current = current->next->next;
		}
		printf("2. currently at node, next: %s, %s\n", current->content, current->next->content);
		if (!before_first && current && current->next
			&& current->next->type >= REDIR_OUT
			&& current->next->type <= HEREDOC)
			before_first = current;
		else if (!before_first && current && current->next
			&& current->next->next
			&& current->type >= REDIR_OUT && current->type <= HEREDOC
			&& (current->next->type == ARG || current->next->type == CMD)
			&& (current->next->next->type == ARG
				|| current->next->next->type == CMD))
		{
			// rearrange_first_time(&current, &before_first);
			rearrange_first_element(&tokens, &current, &before_first);
			continue ;
		}
		else if (before_first && current && current->next
			&& current->next->next
			&& current->type >= REDIR_OUT && current->type <= HEREDOC
			&& (current->next->type == ARG || current->next->type == CMD)
			&& (current->next->next->type == ARG
				|| current->next->next->type == CMD))
		{
			rearrange_tokens(&current, &before_first);
			continue ;
		}
		if (current && (current->type == AND || current->type == OR))
			before_first = NULL;
		current = current->next;
	}
	t_token *temp = tokens;
	while (temp)
	{
		printf("value: %s\n", temp->content);
		temp = temp->next;
	}
	printf("-----------------\n");
	return (tokens);
}

static void	rearrange_first_element(t_token **tokens, t_token **current,
				t_token **before_first)
{
	t_token			*temp[4];

	temp[0] = *tokens;
	temp[1] = (*current)->next;
	temp[2] = (*current)->next->next;
	temp[3] = (*current)->next->next->next;
	*before_first = temp[2];
	(*before_first)->next = temp[0];
	(*current)->next->next = temp[3];
	*tokens = *before_first;
	// *current = *tokens;
}

// static void	rearrange_first_time(t_token **current, t_token **before_first)
// {
// 	t_token			*temp[4];

// 	temp[0] = *current;
// 	temp[1] = (*current)->next;
// 	temp[2] = (*current)->next->next;
// 	temp[3] = (*current)->next->next->next;
// 	*before_first = temp[2];
// 	(*before_first)->next = temp[0];
// 	(*before_first)->next->next = temp[1];
// 	(*before_first)->next->next->next = temp[3];
// }

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
