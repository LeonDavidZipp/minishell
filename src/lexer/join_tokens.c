/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 21:48:04 by intra             #+#    #+#             */
/*   Updated: 2024/02/25 21:48:45 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_treenode	*build_a_s_t(t_token *tokens, int *depth)
{
	t_treenode	*root;

	if (tokens == NULL)
		return (NULL);
	root = new_node(tokens);
	if (tokens->next != NULL)
	{
		root->left = build_a_s_t(tokens->next, depth + 1);
		if (tokens->next->next != NULL)
			root->right = build_a_s_t(tokens->next->next, depth + 1);
	}
	return (root);
}