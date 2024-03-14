/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_tokens_for_redir.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 14:51:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/14 14:44:00 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// static void	rearrange_first_time(t_token **tokens, t_token **current,
// 		t_token **before_first);
// static void	rearrange_tokens_after_first(t_token **current,
// 		t_token **before_first);

t_token	*switch_tokens_for_redir(t_token *tokens)
{
	t_token			*current;
	t_token			*temp[4];
	t_token			*before_first;

	current = tokens;
	before_first = NULL;
	while (current)
	{
		if (!before_first)
		{
			if (current->next && current->next->type >= REDIR_OUT
				&& current->next->type <= HEREDOC)
				before_first = current;
			if (current->type >= REDIR_OUT && current->type <= HEREDOC
				&& current->next && current->next->next)
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
				current = tokens;
				// rearrange_first_time(&tokens, &current, &before_first);
				continue ;
			}
		}
		else if (before_first && current->type >= REDIR_OUT
			&& current->type <= HEREDOC && current->next && current->next->next
			&& (current->next->type == ARG || current->next->type == CMD)
			&& (current->next->next->type == ARG
				|| current->next->next->type == CMD))
		{
			temp[0] = current->next->next;
			temp[1] = before_first->next;
			temp[2] = current->next->next->next;
			before_first->next = temp[0];
			before_first->next->next = temp[1];
			current->next->next = temp[2];
			before_first = before_first->next;
			// rearrange_tokens_after_first(&current, &before_first);
			continue ;
		}
		current = current->next;
	}
	return (tokens);
}

// static void	rearrange_first_time(t_token **tokens, t_token **current,
// 		t_token **before_first)
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
// 	*tokens = *before_first;
// 	*current = *tokens;
// }

// static void	rearrange_tokens_after_first(t_token **current,
// 		t_token **before_first)
// {
// 	t_token			*temp[3];

// 	temp[0] = (*current)->next->next;
// 	temp[1] = (*before_first)->next;
// 	temp[2] = (*current)->next->next->next;
// 	(*before_first)->next = temp[0];
// 	(*before_first)->next->next = temp[1];
// 	(*current)->next->next = temp[2];
// 	*before_first = (*before_first)->next;
// }

// int main(void)
// {
// 	// turn this to tokens: echo > in hi > in2 hell ey you
// 	t_token *tokens = malloc(sizeof(t_token));
// 	tokens->type = CMD;
// 	tokens->content = "echo";
// 	tokens->next = malloc(sizeof(t_token));
// 	tokens->next->type = REDIR_OUT;
// 	tokens->next->content = ">";
// 	tokens->next->next = malloc(sizeof(t_token));
// 	tokens->next->next->type = ARG;
// 	tokens->next->next->content = "in";
// 	tokens->next->next->next = malloc(sizeof(t_token));
// 	tokens->next->next->next->type = ARG;
// 	tokens->next->next->next->content = "hi";
// 	tokens->next->next->next->next = malloc(sizeof(t_token));
// 	tokens->next->next->next->next->type = REDIR_OUT;
// 	tokens->next->next->next->next->content = ">";
// 	tokens->next->next->next->next->next = malloc(sizeof(t_token));
// 	tokens->next->next->next->next->next->type = ARG;
// 	tokens->next->next->next->next->next->content = "in2";
// 	tokens->next->next->next->next->next->next = malloc(sizeof(t_token));
// 	tokens->next->next->next->next->next->next->type = ARG;
// 	tokens->next->next->next->next->next->next->content = "hell";
// 	tokens->next->next->next->next->next->next->next = malloc(sizeof(t_token));
// 	tokens->next->next->next->next->next->next->next->type = ARG;
// 	tokens->next->next->next->next->next->next->next->content = "ey";
// 	tokens->next->next->next->next->next->next->next->next = malloc(sizeof(t_token));
// 	tokens->next->next->next->next->next->next->next->next->type = ARG;
// 	tokens->next->next->next->next->next->next->next->next->content = "you";
// 	tokens->next->next->next->next->next->next->next->next->next = NULL;
// 	tokens = switch_tokens_for_redir(tokens);
// 	while (tokens)
// 	{
// 		printf("%s\n", tokens->content);
// 		tokens = tokens->next;
// 	}
// }
