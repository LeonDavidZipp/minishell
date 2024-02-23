/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:48:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/23 15:12:54 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_treenode	*buildAST(t_token *tokens, int *depth)
{
	t_treenode	*root;

	if (tokens == NULL)
		return (NULL);
	root = new_node(tokens);
	if (tokens->next != NULL)
		root->left = buildAST(tokens->next, depth + 1);
	return (root);
}

static t_treenode	*new_node(t_token *token)
{
	t_treenode	*node;

	node = (t_treenode *)malloc(sizeof(t_treenode));
	if (!node)
		return (NULL);
	node->content = ft_strdup(token->content);
	node->type = token->type;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	debug_print_tabs(int tabs)
{
	while (tabs--)
		printf("\t");
}

void	debug_printtree(t_treenode *root, int tabs)
{
	if (root)
	{
		debug_print_tabs(tabs);
		printf("content: %s, type: %d\n", root->content, root->type);
		debug_print_tabs(tabs);
		printf("left:\n");
		debug_printtree(root->left, tabs + 1);
		debug_print_tabs(tabs);
		printf("right:\n");
		debug_printtree(root->right, tabs + 1);
	}
	else
	{
		debug_print_tabs(tabs);
		printf("--is empty--\n");
	}
}
