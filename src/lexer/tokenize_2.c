/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 11:00:18 by intra             #+#    #+#             */
/*   Updated: 2024/02/27 12:27:31 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	join_tokens_helper(t_token **join, t_token **prev,
				t_token **tokens);

static t_token	*join_arg_tokens(t_token *tokens)
{
	t_token		*first;
	t_token		*prev;
	t_token		*join;

	first = tokens;
	prev = NULL;
	join = NULL;
	while (tokens)
	{
		if (tokens->type == ARG || tokens->type == FLAG)
			join_tokens_helper(&join, &prev, &tokens);
		else if (tokens->type != ARG)
		{
			join = NULL;
			prev = tokens;
			tokens = tokens->next;
		}
		if (prev)
			prev->next = tokens;
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
		*prev = *tokens;
		*tokens = (*tokens)->next;
	}
}

static t_token	*join_after_echo(t_token *tokens)
{
	t_token		*first;
	t_token		*prev;
	t_token		*join;
	int			echo_flag;

	first = tokens;
	prev = NULL;
	join = NULL;
	echo_flag = 0;
	while (tokens)
	{
		if (echo_flag == 0 && ft_strcmp(tokens->content, "echo") == 0)
			echo_flag = 1;
		else if (echo_flag == 1 && (tokens->type == PIPE
				|| tokens->type == AND || tokens->type == OR
				|| tokens->type == REDIR_OUT || tokens->type == REDIR_IN
				|| tokens->type == REDIR_APPEND || tokens->type == HEREDOC))
			echo_flag = 0;
		else if (echo_flag == 1 && (tokens->type == ARG
				|| tokens->type == FLAG || tokens->type == WILDCARD
				|| tokens->type == BUILTIN_CMD || tokens->type == OTHER_CMD))
			join_tokens_helper(&join, &prev, &tokens);
		else
		{
			join = NULL;
			prev = tokens;
			tokens = tokens->next;
		}
		if (prev)
			prev->next = tokens;
		if (first == tokens && tokens->type == ARG)
			first = join;
	}
}
