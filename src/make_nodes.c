/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:48:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/14 12:55:18 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_treenode	*new_node(t_input *input)
{
	t_treenode	*node;

	node = (t_treenode *)malloc(sizeof(t_treenode));
	if (!node)
		return (NULL);
	node->content = input->content;
	node->type = input->type;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	debug_printtree(t_treenode *root)
{
	if (root)
	{
		printf("content: %s, type: %d\n", root->content, root->type);
		debug_printtree(root->left);
		debug_printtree(root->right);
	}
}

