/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:48:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/04 14:26:55 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// static t_treenode	*build_subtree(t_treenode **temp);
static t_treenode	*new_treenode(char *cmd, char *args, int bracket_lvl);
static t_treenode	*insert_node(t_treenode *root, t_treenode *node);



t_treenode	*build_ast(t_treenode *ast, t_treenode *lin_tree, int bracket_lvl)
{
	t_treenode	*new;

	// this means all nodes have been traversed and the ast is readily built
	if (!lin_tree)
		return (ast);
	// if the bracket lvl didnt increase, just insert the new node at the right place
	if (lin_tree->bracket_lvl <= bracket_lvl)
	{
		new = new_treenode(lin_tree->cmd, lin_tree->args, lin_tree->bracket_lvl);
		ast = insert_node(ast, new);
		ast = build_ast(ast, lin_tree->left, lin_tree->bracket_lvl);
	}
	else if (lin_tree->bracket_lvl > bracket_lvl)
	{
		ast = build_ast(ast, lin_tree->left, lin_tree->bracket_lvl);
	}
	return (ast);
}

t_treenode *build_subtree(t_treenode *sub, t_treenode **lin_tree, int bracket_lvl)
{
	t_treenode	*new;

	if (!(*lin_tree))
		return (sub);
	// if the bracket lvl didnt increase, just insert the new node at the right place
	if ((*lin_tree)->bracket_lvl == bracket_lvl)
	{
		new = new_treenode((*lin_tree)->cmd, (*lin_tree)->args, (*lin_tree)->bracket_lvl);
		sub = insert_node(sub, new);
		sub = build_subtree(sub, (*lin_tree)->left, (*lin_tree)->bracket_lvl);
	}
}

static t_treenode	*new_treenode(char *cmd, char *args, int bracket_lvl)
{
	t_treenode		*node;

	node = malloc(sizeof(t_treenode));
	node->cmd = ft_strdup(cmd);
	node->args = ft_strdup(args);
	node->bracket_lvl = bracket_lvl;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	free_treenodes(t_treenode *node)
{
	if (node->left)
		free_treenodes(node->left);
	if (node->right)
		free_treenodes(node->right);
	free(node->cmd);
	free(node->args);
	free(node);
}

static t_treenode	*insert_node(t_treenode *root, t_treenode *node)
{
	// If the tree is empty, assign a new node address to root
	if (!root)
		return (node);
	// If the command is an operator and has higher precedence than the root
	if (node_is_operator(node->cmd) && priority(node->cmd) >= priority(root->cmd))
	{
		node->left = root;
		return (node);
	}
	// Else, recursively insert the node into the right subtree
	root->right = insert_node(root->right, node);
	return (root);
}

int	main(void)
{
	t_app_data	app;
	t_token		*tokens;
	t_treenode	*root;
	t_treenode	*ast;

	app.input = ft_strdup("echo 1 || echo 2 && echo 3");
	tokens = tokenize(&app);
	root = combine_cmds_args(tokens);
	ast = NULL;
	ast = build_ast(ast, root, 0);
	debug_printtree(ast, 0);
	return (0);
}
