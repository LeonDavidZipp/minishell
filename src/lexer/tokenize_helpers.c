/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 11:00:18 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/10 21:34:07 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	join_tokens_helper(t_token **join, t_token **prev,
				t_token **tokens);
static void	join_after_echo_loop(int *echo_flag, t_token **tokens,
				t_token **join, t_token **prev);

t_token	*join_arg_tokens(t_token *tokens)
{
	t_token		*first;
	t_token		*prev;
	t_token		*join;

	first = tokens;
	prev = NULL;
	join = NULL;
	while (tokens)
	{
		if (tokens->type == ARG)
			join_tokens_helper(&join, &prev, &tokens);
		else if (tokens->type != ARG)
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

t_token	*join_after_echo(t_token *tokens)
{
	t_token		*first;
	t_token		*prev;
	t_token		*join;
	int			echo_flag;

	first = tokens;
	prev = NULL;
	join = NULL;
	echo_flag = 0;
	join_after_echo_loop(&echo_flag, &tokens, &join, &prev);
	return (first);
}

static void	join_after_echo_loop(int *echo_flag, t_token **tokens,
				t_token **join, t_token **prev)
{
	while (*tokens)
	{
		if (*echo_flag == 0 && ft_strcmp((*tokens)->content, "echo") == 0)
			*echo_flag = 1;
		else if (*echo_flag == 1)
		{
			if ((*tokens)->type == REDIR_APPEND || (*tokens)->type == AND
				|| (*tokens)->type == OR || (*tokens)->type == REDIR_OUT
				|| (*tokens)->type == REDIR_IN || (*tokens)->type == PIPE
				|| (*tokens)->type == HEREDOC || (*tokens)->type == LEFT_BRACKET
				|| (*tokens)->type == RIGHT_BRACKET)
				*echo_flag = 0;
			else
			{
				join_tokens_helper(join, prev, tokens);
				continue ;
			}
		}
		else
			*echo_flag = 0;
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

t_token	*switch_args_for_redir(t_token *token)
{
	t_token	*current;
	t_token	*temp[4];

	current = token;
	while (current && current->next && current->next->next
		&& current->next->next->next)
	{
		if ((current->next->type == REDIR_OUT || current->next->type == REDIR_IN
				|| current->next->type == REDIR_APPEND
				|| current->next->type == HEREDOC)
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

// int	main(void)
// {
// 	t_token	*token1 = malloc(sizeof(t_token));
// 	token1->content = ft_strdup("cd");
// 	token1->type = CMD;
// 	token1->next = malloc(sizeof(t_token));
// 	token1->next->content = ft_strdup("hello");
// 	token1->next->type = ARG;
// 	token1->next->next = malloc(sizeof(t_token));
// 	token1->next->next->content = ft_strdup("echo");
// 	token1->next->next->type = CMD;
// 	token1->next->next->next = malloc(sizeof(t_token));
// 	token1->next->next->next->content = ft_strdup("hi");
// 	token1->next->next->next->type = ARG;
// 	token1->next->next->next->next = malloc(sizeof(t_token));
// 	token1->next->next->next->next->content = ft_strdup("cd");
// 	token1->next->next->next->next->type = CMD;
// 	token1->next->next->next->next->next = malloc(sizeof(t_token));
// 	token1->next->next->next->next->next->content = ft_strdup("&&");
// 	token1->next->next->next->next->next->type = AND;
// 	token1->next->next->next->next->next->next = NULL;
// 	t_token	*token2 = join_after_echo(token1);
// 	while (token2)
// 	{
// 		printf("|%s|\n", token2->content);
// 		token2 = token2->next;
// 	}
// 	return (0);
// }
