/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_after_ls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 11:00:18 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/08 11:24:07 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	join_tokens_helper(t_token **join, t_token **prev,
				t_token **tokens);
static void	join_after_ls_loop(int *ls_flag, t_token **tokens,
				t_token **join, t_token **prev);

t_token	*join_after_ls(t_token *tokens)
{
	t_token		*first;
	t_token		*prev;
	t_token		*join;
	int			ls_flag;

	first = tokens;
	prev = NULL;
	join = NULL;
	ls_flag = 0;
	join_after_ls_loop(&ls_flag, &tokens, &join, &prev);
	return (first);
}

static void	join_after_ls_loop(int *ls_flag, t_token **tokens,
				t_token **join, t_token **prev)
{
	while (*tokens)
	{
		if (*ls_flag == 0 && ft_strcmp((*tokens)->content, "ls") == 0)
			*ls_flag = 1;
		else if (*ls_flag == 1)
		{
			if ((*tokens)->type == REDIR_APPEND || (*tokens)->type == AND
				|| (*tokens)->type == OR || (*tokens)->type == REDIR_OUT
				|| (*tokens)->type == REDIR_IN || (*tokens)->type == PIPE
				|| (*tokens)->type == HEREDOC || (*tokens)->type == LEFT_BRACKET
				|| (*tokens)->type == RIGHT_BRACKET)
				*ls_flag = 0;
			else
			{
				join_tokens_helper(join, prev, tokens);
				continue ;
			}
		}
		else
			*ls_flag = 0;
		*join = NULL;
		*prev = *tokens;
		(*prev)->next = (*tokens)->next;
		*tokens = (*tokens)->next;
	}
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
