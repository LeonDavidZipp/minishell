/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_arg_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 11:00:18 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/11 16:58:41 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	join_tokens_helper(t_token **join, t_token **prev,
				t_token **tokens);

t_token	*join_arg_tokens(t_token *tokens)
{
	t_token		*first;
	t_token		*prev;
	t_token		*join;
	bool 		prev_cmd;

	first = tokens;
	prev = NULL;
	join = NULL;
	prev_cmd = false;
	while (tokens)
	{
		if (tokens->type == CMD && prev_cmd == false)
		{
			prev_cmd = true;
			tokens = tokens->next;
			if (!tokens)
				break ;
		}
		// if (node_is_operator(tokens->content))
		if (node_is_operator(tokens->type))
			prev_cmd = false;
		if (prev_cmd == true)
			join_tokens_helper(&join, &prev, &tokens);
		else
		{
			join = NULL;
			prev = tokens;
			prev->next = tokens->next;
			tokens = tokens->next;
		}
		if (first == tokens && tokens->type == ARG)
			first = join;
	}
	return (first);
}

static void	join_tokens_helper(t_token **join, t_token **prev,
	t_token **tokens)
{
	if (*join)
	{
		(*join)->content = ft_join_in_place((*join)->content, " ");
		(*join)->content = ft_join_in_place((*join)->content,
				(*tokens)->content);
		(*join)->next = (*tokens)->next;
		free((*tokens)->content);
		free(*tokens);
		*tokens = (*join)->next;
	}
	else
	{
		*join = *tokens;
		(*join)->type = ARG;
		*prev = *tokens;
		(*prev)->next = (*tokens)->next;
		*tokens = (*tokens)->next;
	}
}
