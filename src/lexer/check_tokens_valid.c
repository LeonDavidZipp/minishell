/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens_valid.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 22:01:18 by intra             #+#    #+#             */
/*   Updated: 2024/02/26 12:05:44 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_tokens_valid(t_token *tokens)
{
	if (tokens->type == FLAG || tokens->type == AND || tokens->type == OR
		|| tokens->type == REDIR_OUT || tokens->type == REDIR_IN
		|| tokens->type == REDIR_APPEND || tokens->type == HEREDOC
		|| tokens->type == PIPE)
		return (0);
	tokens = tokens->next;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!tokens->next || tokens->next->type == PIPE
				|| tokens->next->type == FLAG || tokens->next->type == AND
				|| tokens->next->type == OR || tokens->next->type == WILDCARD
				|| tokens->next->type == ARG)
				return (0);
		}
		else if (tokens->type == ARG)
		{
			if (tokens->next && (tokens->next->type == REDIR_OUT
				|| tokens->next->type == REDIR_IN || tokens->next->type == REDIR_APPEND
				|| tokens->next->type == HEREDOC))
				return (0);
		}
		else if (tokens->type == FLAG || tokens->type == AND || tokens->type == OR
				|| tokens->type == REDIR_OUT || tokens->next->type == REDIR_IN
				|| tokens->next->type == REDIR_APPEND || tokens->next->type == HEREDOC)
		{
			if (!tokens->next || tokens->next->type == FLAG || tokens->next->type == AND
				|| tokens->next->type == OR || tokens->next->type == WILDCARD)
				return (0);
		}
		if (tokens->type == FLAG)
		{
			if (!tokens->next || tokens->next->type == PIPE || tokens->next->type == AND
				|| tokens->next->type == OR || tokens->next->type == REDIR_OUT
				|| tokens->next->type == REDIR_IN || tokens->next->type == REDIR_APPEND
				|| tokens->next->type == HEREDOC)
				return (0);
		}
		else if (tokens->type == SINGLE_QUOTE || tokens->type == DOUBLE_QUOTE)
		{
			if (!tokens->next || tokens->next->type == PIPE || tokens->next->type == AND
				|| tokens->next->type == OR || tokens->next->type == REDIR_OUT
				|| tokens->next->type == REDIR_IN || tokens->next->type == REDIR_APPEND
				|| tokens->next->type == HEREDOC || tokens->next->type == WILDCARD)
				return (0);
		}
		else if (tokens->type == AND || tokens->type == OR)
		{
			if (!tokens->next || tokens->next->type == PIPE || tokens->next->type == AND
				|| tokens->next->type == OR || tokens->next->type == WILDCARD)
				return (0);
		}
		else if (tokens->type == REDIR_OUT || tokens->type == REDIR_IN || tokens->type == REDIR_APPEND)
		{
			if (!tokens->next || tokens->next->type == PIPE || tokens->next->type == AND
				|| tokens->next->type == OR || tokens->next->type == WILDCARD
				|| tokens->next->type == REDIR_OUT || tokens->next->type == REDIR_IN
				|| tokens->next->type == REDIR_APPEND || tokens->next->type == HEREDOC)
				return (0);
		}
		else if (tokens->type == HEREDOC)
		{
			if (!tokens->next || tokens->next->type == PIPE || tokens->next->type == AND
				|| tokens->next->type == OR || tokens->next->type == WILDCARD
				|| tokens->next->type == REDIR_OUT || tokens->next->type == REDIR_IN
				|| tokens->next->type == REDIR_APPEND || tokens->next->type == HEREDOC)
				return (0);
		}
		else if (tokens->type == WILDCARD)
		{
			if (!tokens->next || tokens->next->type == PIPE || tokens->next->type == AND
				|| tokens->next->type == OR || tokens->next->type == REDIR_OUT
				|| tokens->next->type == REDIR_IN || tokens->next->type == REDIR_APPEND
				|| tokens->next->type == HEREDOC)
				return (0);
		}
		else if (tokens->type == BUILTIN_CMD || tokens->type == OTHER_CMD)
		{
			if (!tokens->next || tokens->next->type == BUILTIN_CMD || tokens->next->type == OTHER_CMD)
				return (0);
		}
		tokens = tokens->next;
	}
	return (1);
}
