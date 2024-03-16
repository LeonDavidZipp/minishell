/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_tokens_for_redir.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 14:51:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/16 16:23:28 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	skip_tokens(t_token **before_first, t_token **current);
static void	rearrange_first_element(t_token **tokens, t_token **current,
				t_token **before_first);
static void	rearrange_following_element(t_token **current,
				t_token **before_first);

t_token	*switch_tokens_for_redir(t_token *tokens)
{
	t_token			*current;
	bool			first_rearrange;
	t_token			*before_first;

	current = tokens;
	first_rearrange = true;
	before_first = NULL;
	if (current->next && current->next->type >= REDIR_OUT
		&& current->next->type <= HEREDOC)
		before_first = current;
	while (current)
	{
		skip_tokens(&before_first, &current);
		if (current->type == AND || current->type == OR)
		{
			printf("encountered AND or OR\n");
			first_rearrange = false;
			before_first = NULL;
		}
		if (!before_first && current && current->next
			&& current->next->next
			&& current->type >= REDIR_OUT && current->type <= HEREDOC
			&& (current->next->type == ARG || current->next->type == CMD)
			&& (current->next->next->type == ARG
				|| current->next->next->type == CMD))
		{
			rearrange_first_element(&tokens, &current, &before_first);
			if (first_rearrange)
				tokens = before_first;
			continue ;
		}
		else if (before_first && current && current->next
			&& current->next->next
			&& current->type >= REDIR_OUT && current->type <= HEREDOC
			&& (current->next->type == ARG || current->next->type == CMD)
			&& (current->next->next->type == ARG
				|| current->next->next->type == CMD))
		{
			rearrange_following_element(&current, &before_first);
			continue ;
		}
		current = current->next;
	}
	return (tokens);
}

static void	skip_tokens(t_token **before_first, t_token **current)
{
	while (!*before_first && (*current)->next
		&& !((*current)->type >= REDIR_OUT && (*current)->type <= HEREDOC)
		&& !((*current)->next->type >= REDIR_OUT
			&& (*current)->next->type <= HEREDOC))
		*current = (*current)->next;
	while (*current && (*current)->next && (*current)->next->next
		&& (((*current)->type >= REDIR_OUT && (*current)->type <= HEREDOC
		&& ((*current)->next->type == CMD || (*current)->next->type == ARG)
			&& (*current)->next->next->type != CMD
				&& (*current)->next->next->type != ARG)))
		*current = (*current)->next->next;
}

static void	rearrange_first_element(t_token **tokens, t_token **current,
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

static void	rearrange_following_element(t_token **current, t_token **before_first)
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
