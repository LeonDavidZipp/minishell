/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens_valid.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 22:01:18 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/23 13:40:46 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	check_and_or_pipe_redir_out_append(t_token *current);
static bool	check_redir_in_heredoc(t_token *current);
static bool	check_echo(t_token *current);

bool	check_tokens_valid(t_token *tokens)
{
	t_token		*current;

	if (!tokens)
		return (true);
	if (tokens->type == OR || tokens->type == AND || tokens->type == PIPE)
	{
		return (ft_fprintf(2, "%s: %s '%s'\n",
				NAME, SYNTAX_ERR_MSG, tokens->content), false);
	}
	current = tokens;
	while (current)
	{
		if (!check_and_or_pipe_redir_out_append(current))
			return (false);
		if (!check_redir_in_heredoc(current))
			return (false);
		if (!check_echo(current))
			return (false);
		current = current->next;
	}
	return (true);
}

static bool	check_and_or_pipe_redir_out_append(t_token *current)
{
	if (current->type == AND || current->type == OR || current->type == PIPE)
	{
		if (!current->next)
			return (ft_fprintf(2, "%s: %s '\\n'\n", NAME), false);
		else if (current->next->type == AND || current->next->type == OR
			|| current->next->type == PIPE)
			return (ft_fprintf(2, "%s: %s '%s'\n",
					NAME, SYNTAX_ERR_MSG, current->next->content), false);
	}
	else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
	{
		if (!current->next)
			return (ft_fprintf(2, "%s: %s '\\n'\n", NAME), false);
		else if (current->next->type == AND || current->next->type == OR
			|| current->next->type == PIPE || current->next->type == REDIR_IN
			|| current->next->type == REDIR_OUT
			|| current->next->type == REDIR_APPEND
			|| current->next->type == HEREDOC)
			return (ft_fprintf(2, "%s: %s '%s'\n",
					NAME, SYNTAX_ERR_MSG, current->next->content), false);
	}
	return (true);
}

static bool	check_redir_in_heredoc(t_token *current)
{
	if (current->type == REDIR_IN || current->type == HEREDOC)
	{
		if (!current->next)
			return (ft_fprintf(2, "%s: %s '\\n'\n", NAME), false);
		if (current->next->type == AND || current->next->type == OR
			|| current->next->type == PIPE || current->next->type == REDIR_IN
			|| current->next->type == REDIR_OUT
			|| current->next->type == REDIR_APPEND
			|| current->next->type == HEREDOC)
			return (ft_fprintf(2, "%s: %s '%s'\n",
					NAME, SYNTAX_ERR_MSG, current->next->content), false);
	}
	return (true);
}

static bool	check_echo(t_token *current)
{
	if (ft_strcmp(current->content, "echo") == 0)
	{
		if (current->next && current->next->next
			&& (current->next->type == LEFT_BRACKET
				|| current->next->type == RIGHT_BRACKET))
		{
			ft_fprintf(2, "%s: %s \'%s\'\n",
				NAME, SYNTAX_ERR_MSG, current->next->next->content);
			return (false);
		}
		else if (current->next && (current->next->type == LEFT_BRACKET
				|| current->next->type == RIGHT_BRACKET))
		{
			ft_fprintf(2, "%s: %s '\\n'\n", NAME, SYNTAX_ERR_MSG);
			return (false);
		}
	}
	return (true);
}
