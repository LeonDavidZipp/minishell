/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_tokens_for_redir.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 14:51:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/18 17:14:56 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void		skip_tokens(t_token **before_first, t_token **current);
static void		rearrange_first_element(t_token **tokens, t_token **current,
					t_token **before_first);
static void		rearrange_following_element(t_token **current,
					t_token **before_first);
static t_token	*reassign_type(t_token *token);
static bool	rearrange(bool *first_rearrange, t_token **before_first,
					t_token **tokens, t_token **current);
static bool		rearrange_condition(t_token *current);
static bool		before_first_condition(t_token *current);

t_token	*switch_tokens_for_redir(t_token *tokens)
{
	t_token			*current;
	bool			first_rearrange;
	t_token			*before_first;

	current = tokens;
	first_rearrange = true;
	before_first = NULL;
	while (current)
	{
		skip_tokens(&before_first, &current);
		if (!before_first && before_first_condition(current))
			before_first = current;
		if (rearrange(&first_rearrange, &before_first, &tokens, &current))
			continue ;
		if (current && (current->type == AND || current->type == OR))
		{
			before_first = NULL;
			first_rearrange = false;
			if (before_first_condition(current))
				before_first = current;
		}
		current = current->next;
	}
	return (reassign_type(tokens));
}

static bool	rearrange(bool *first_rearrange, t_token **before_first, t_token **tokens,
		t_token **current)
{
	if (!*before_first && rearrange_condition(*current))
	{
		rearrange_first_element(tokens, current, before_first);
		if (*first_rearrange)
		{
			*tokens = *before_first;
			*first_rearrange = false;
		}
		return true;
	}
	else if (*before_first && rearrange_condition(*current))
	{
		rearrange_following_element(current, before_first);
		return true;
	}
	return false;
}

static bool	rearrange_condition(t_token *current)
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

static bool	before_first_condition(t_token *current)
{
	if (current && current->next
		&& current->next->type >= REDIR_OUT
		&& current->next->type <= HEREDOC)
		return (true);
	return (false);
}

static void	skip_tokens(t_token **before_first, t_token **current)
{
	while (!*before_first && (*current) && (*current)->next
		&& !((*current)->type >= REDIR_OUT && (*current)->type <= HEREDOC)
		&& !((*current)->next->type >= REDIR_OUT
			&& (*current)->next->type <= HEREDOC))
		*current = (*current)->next;
	while (*current && (*current)->next && (*current)->next->next
		&& (((*current)->type >= REDIR_OUT && (*current)->type <= HEREDOC
				&& ((*current)->next->type == CMD
					|| (*current)->next->type == ARG)
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

static t_token	*reassign_type(t_token *tokens)
{
	t_token	*current;
	t_token	*prev;

	current = tokens;
	prev = NULL;
	while (current)
	{
		if (prev)
			current->type = token_type(current->content, prev->type);
		else
			current->type = token_type(current->content, FIRST);
		prev = current;
		current = current->next;
	}
	return (tokens);
}
