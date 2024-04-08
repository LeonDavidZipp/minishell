/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens_valid.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 22:01:18 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/08 16:26:37 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_and_or_pipe_redir_out_append(t_token *current);
static int	check_redir_in_heredoc(t_token *current);
static int	check_echo(t_token *current);
static int	check_brackets(t_token *current);

int	check_tokens_valid(t_token *tokens)
{
	t_token		*current;
	int			exit_code;

	if (tokens->type == OR || tokens->type == AND || tokens->type == PIPE)
		return (ft_fprintf(2, "%s: %s '%s'\n", NAME, SYNT_MSG, tokens->content),
			258);
	if (ft_strcmp(tokens->content, ".") == 0 && !tokens->next)
		return (ft_fprintf(2, "%s%s%s", NAME, DOT_MSG, DOT_MSG2), 127);
	current = tokens;
	exit_code = 0;
	while (current && exit_code == 0)
	{
		if (current->type == AND || current->type == OR || current->type == PIPE
			|| current->type == REDIR_OUT || current->type == REDIR_APPEND)
			exit_code = check_and_or_pipe_redir_out_append(current);
		else if (current->type == REDIR_IN || current->type == HEREDOC)
			exit_code = check_redir_in_heredoc(current);
		else if (ft_strcmp(current->content, "echo") == 0)
			exit_code = check_echo(current);
		else if (current->type == LEFT_BRACKET || current->type == RIGHT_BRACKET)
			exit_code = check_brackets(current);
		current = current->next;
	}
	return (exit_code);
}

static int	check_and_or_pipe_redir_out_append(t_token *current)
{
	if (current->type == AND || current->type == OR || current->type == PIPE)
	{
		if (!current->next)
			return (ft_fprintf(2, "%s: %s 'newline'\n", NAME, SYNT_MSG), 2);
		else if (current->next->type == AND || current->next->type == OR
			|| current->next->type == PIPE)
			return (ft_fprintf(2, "%s: %s '%s'\n",
					NAME, SYNT_MSG, current->next->content), 2);
	}
	else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
	{
		if (!current->next)
			return (ft_fprintf(2, "%s: %s 'newline'\n", NAME, SYNT_MSG), 2);
		else if (current->next->type == AND || current->next->type == OR
			|| current->next->type == PIPE || current->next->type == REDIR_IN
			|| current->next->type == REDIR_OUT
			|| current->next->type == REDIR_APPEND
			|| current->next->type == HEREDOC)
			return (ft_fprintf(2, "%s: %s '%s'\n",
					NAME, SYNT_MSG, current->next->content), 2);
	}
	return (0);
}

static int	check_redir_in_heredoc(t_token *current)
{
	if (current->type == REDIR_IN || current->type == HEREDOC)
	{
		if (!current->next)
			return (ft_fprintf(2, "%s: %s 'newline'\n", NAME, SYNT_MSG), 2);
		if (current->next->type == AND || current->next->type == OR
			|| current->next->type == PIPE || current->next->type == REDIR_IN
			|| current->next->type == REDIR_OUT
			|| current->next->type == REDIR_APPEND
			|| current->next->type == HEREDOC)
			return (ft_fprintf(2, "%s: %s '%s'\n",
					NAME, SYNT_MSG, current->next->content), 258);
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
				NAME, SYNT_MSG, current->next->next->content);
			return (258);
		}
		else if (current->next && (current->next->type == LEFT_BRACKET
				|| current->next->type == RIGHT_BRACKET))
		{
			ft_fprintf(2, "%s: %s 'newline'\n", NAME, SYNT_MSG);
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
			return (ft_fprintf(2, "%s: %s 'newline'\n", NAME, SYNT_MSG), 258);
		else if (current->next->type == AND || current->next->type == OR
			|| current->next->type == PIPE || current->next->type == REDIR_IN
			|| current->next->type == REDIR_OUT
			|| current->next->type == REDIR_APPEND
			|| current->next->type == HEREDOC || current->next->type == RIGHT_BRACKET)
			return (ft_fprintf(2, "%s: %s '%s'\n",
					NAME, SYNT_MSG, current->next->content), 258);
	}
	else if (current->type == RIGHT_BRACKET)
	{
		if (current->next && (current->next->type == CMD || current->next->type == ARG
			|| current->next->type == LEFT_BRACKET))
			return (ft_fprintf(2, "%s: %s '%s'\n",
					NAME, SYNT_MSG, current->next->content), 258);
	}
	return (0);
}
