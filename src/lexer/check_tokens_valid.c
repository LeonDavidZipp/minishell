/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens_valid.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 22:01:18 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/15 17:25:13 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_and_or_pipe_redir_out_append(t_token *current);
static int	check_redir_in_heredoc(t_token *current);
static int	check_echo(t_token *current);
static int	check_brackets(t_token *current);

int	check_tokens_valid(t_token *t, int *err_loc)
{
	t_token		*cur;
	int			exit_code;

	if (t->type == OR || t->type == AND || t->type == PIPE)
		return (ft_fprintf(2, "%s: %s '%s'\n", NAME, SYN_MSG, t->content), 258);
	cur = t;
	exit_code = 0;
	while (cur && exit_code == 0)
	{
		if (cur->type == AND || cur->type == OR || cur->type == PIPE
			|| cur->type == REDIR_OUT || cur->type == REDIR_APPEND)
			exit_code = check_and_or_pipe_redir_out_append(cur);
		else if (cur->type == REDIR_IN || cur->type == HEREDOC)
			exit_code = check_redir_in_heredoc(cur);
		else if (ft_strcmp(cur->content, "echo") == 0)
			exit_code = check_echo(cur);
		else if (cur->type == LEFT_BRACKET || cur->type == RIGHT_BRACKET)
			exit_code = check_brackets(cur);
		cur = cur->next;
		(*err_loc)++;
	}
	return (exit_code);
}

static int	check_and_or_pipe_redir_out_append(t_token *cur)
{
	if (cur->type == AND || cur->type == OR || cur->type == PIPE)
	{
		if (!cur->next)
			return (ft_fprintf(2, "%s: %s 'newline'\n", NAME, SYN_MSG), 2);
		else if (cur->next->type == AND || cur->next->type == OR
			|| cur->next->type == PIPE)
			return (ft_fprintf(2, "%s: %s '%s'\n",
					NAME, SYN_MSG, cur->next->content), 2);
	}
	else if (cur->type == REDIR_OUT && cur->next && cur->next->type == PIPE)
		return (ft_fprintf(2, "%s: %s '%s'\n",
				NAME, SYN_MSG, cur->next->content), 127);
	else if (cur->type == REDIR_OUT || cur->type == REDIR_APPEND)
	{
		if (!cur->next)
			return (ft_fprintf(2, "%s: %s 'newline'\n", NAME, SYN_MSG), 2);
		else if (cur->next->type == AND || cur->next->type == OR
			|| cur->next->type == PIPE || cur->next->type == REDIR_IN
			|| cur->next->type == REDIR_OUT
			|| cur->next->type == REDIR_APPEND
			|| cur->next->type == HEREDOC)
			return (ft_fprintf(2, "%s: %s '%s'\n",
					NAME, SYN_MSG, cur->next->content), 2);
	}
	return (0);
}

static int	check_redir_in_heredoc(t_token *current)
{
	if (current->type == REDIR_IN || current->type == HEREDOC)
	{
		if (!current->next)
			return (ft_fprintf(2, "%s: %s 'newline'\n", NAME, SYN_MSG), 2);
		if (current->next->type == AND || current->next->type == OR
			|| current->next->type == PIPE || current->next->type == REDIR_IN
			|| current->next->type == REDIR_OUT
			|| current->next->type == REDIR_APPEND
			|| current->next->type == HEREDOC)
			return (ft_fprintf(2, "%s: %s '%s'\n",
					NAME, SYN_MSG, current->next->content), 258);
	}
	return (0);
}

static int	check_echo(t_token *current)
{
	if (ft_strcmp(current->content, "echo") == 0)
	{
		if (current->next && current->next->next
			&& (current->next->type == LEFT_BRACKET
				|| current->next->type == RIGHT_BRACKET))
		{
			ft_fprintf(2, "%s: %s \'%s\'\n",
				NAME, SYN_MSG, current->next->next->content);
			return (258);
		}
		else if (current->next && (current->next->type == LEFT_BRACKET
				|| current->next->type == RIGHT_BRACKET))
		{
			ft_fprintf(2, "%s: %s 'newline'\n", NAME, SYN_MSG);
			return (258);
		}
	}
	return (0);
}

static int	check_brackets(t_token *current)
{
	if (current->type == LEFT_BRACKET)
	{
		if (!current->next)
			return (ft_fprintf(2, "%s: %s 'newline'\n", NAME, SYN_MSG), 258);
		else if (current->next->type == AND || current->next->type == OR
			|| current->next->type == PIPE || current->next->type == REDIR_IN
			|| current->next->type == REDIR_OUT
			|| current->next->type == REDIR_APPEND
			|| current->next->type == HEREDOC
			|| current->next->type == RIGHT_BRACKET)
			return (ft_fprintf(2, "%s: %s '%s'\n",
					NAME, SYN_MSG, current->next->content), 258);
	}
	else if (current->type == RIGHT_BRACKET)
	{
		if (current->next && (current->next->type == CMD
				|| current->next->type == ARG
				|| current->next->type == LEFT_BRACKET))
			return (ft_fprintf(2, "%s: %s '%s'\n",
					NAME, SYN_MSG, current->next->content), 258);
	}
	return (0);
}
