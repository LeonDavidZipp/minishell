/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens_valid.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 22:01:18 by intra             #+#    #+#             */
/*   Updated: 2024/02/28 13:55:16 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	check_first_last_node(t_treenode *node);

bool	check_nodes_valid(t_treenode *nodes)
{
	t_treenode	*temp;
	t_tokentype	type;

	if (!check_first_last_node(nodes))
		return (false);	
	temp = nodes->left;
	while (temp->left)
	{
		type = token_type(temp->cmd);
		if (type == ARG || type == WILDCARD)
			return (false);
		if ((type == PIPE || type == AND || type == OR)
				&& temp->args != NULL)
			return (false);
		temp = temp->left;
	}
	if (!check_first_last_node(temp))
		return (false);
	return (true);
}

static bool	check_first_last_node(t_treenode *node)
{
	if (token_type(node->cmd) == ARG
		|| token_type(node->cmd) == OR
		|| token_type(node->cmd) == AND
		|| token_type(node->cmd) == PIPE
		|| token_type(node->cmd) == REDIR_OUT
		|| token_type(node->cmd) == REDIR_IN
		|| token_type(node->cmd) == REDIR_APPEND
		|| token_type(node->cmd) == HEREDOC)
		return (false);
}	
