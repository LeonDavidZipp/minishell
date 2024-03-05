/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:48:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/04 22:40:21 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// static t_treenode	*build_subtree(t_treenode **temp);
t_treenode			*build_subtree(t_treenode *sub, t_treenode **lin_tree,
								int bracket_lvl);
t_treenode			*build_subtree_2(t_treenode *sub, t_treenode **lin_tree,
								int bracket_lvl);
static t_treenode	*new_treenode(char *cmd, char *args, int bracket_lvl);
static t_treenode	*insert_node(t_treenode *root, t_treenode *node);

t_treenode	*build_ast(t_treenode *ast, t_treenode *lin_tree, int bracket_lvl)
{
	t_treenode	*new;

	// correct
	if (!lin_tree)
		return (ast);
	if (lin_tree->bracket_lvl <= bracket_lvl)
	{
		new = new_treenode(lin_tree->cmd, lin_tree->args, lin_tree->bracket_lvl);
		ast = insert_node(ast, new);
		ast = build_ast(ast, lin_tree->left, lin_tree->bracket_lvl);
	}
	// correct
	else if (lin_tree->bracket_lvl > bracket_lvl)
	{
		new = build_subtree(NULL, &lin_tree, lin_tree->bracket_lvl);
		ast = insert_node(ast, new);
		if (!lin_tree)
			return (ast);
		ast = build_ast(ast, lin_tree, lin_tree->bracket_lvl);
	}
	return (ast);
}


t_treenode	*build_subtree(t_treenode *sub, t_treenode **lin_tree, int bracket_lvl)
{
	t_treenode	*new;

	if (!(*lin_tree) || (*lin_tree)->bracket_lvl < bracket_lvl)
	{
		return (sub);
	}
	// printf("cmd: %s, args: %s, lvl: %d\n", (*lin_tree)->cmd, (*lin_tree)->args, (*lin_tree)->bracket_lvl);
	if ((*lin_tree)->bracket_lvl == bracket_lvl)
	{
		new = new_treenode((*lin_tree)->cmd, (*lin_tree)->args, (*lin_tree)->bracket_lvl);
		sub = insert_node(sub, new);
		debug_printtree(sub, 0);
		printf("\n-----------------\n");
		*lin_tree = (*lin_tree)->left;
		sub = build_subtree(sub, lin_tree, bracket_lvl);
	}
	else
	{
		printf("building subtree\n");
		new = build_subtree(NULL, lin_tree, (*lin_tree)->bracket_lvl);
		printf("inserting node\n");
		sub = insert_node(sub, new);
		if (!(*lin_tree))
			return (sub);
		sub = build_subtree(sub, lin_tree, (*lin_tree)->bracket_lvl);
	}
	return (sub);
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
	if (node_is_operator(node->cmd) && (priority(node->cmd) >= priority(root->cmd)))
	{
		node->left = root;
		return (node);
	}
	// Else, recursively insert the node into the right subtree
	root->right = insert_node(root->right, node);
	return (root);
}

// static t_treenode	*insert_node(t_treenode *root, t_treenode *node)
// {
// 	// If the tree is empty, assign a new node address to root
// 	if (!root)
// 		return (node);
// 	// If the command is an operator and has higher precedence than the root
// 	// or the node has a higher bracket level than the root
// 	if (node_is_operator(node->cmd) && (priority(node->cmd) >= priority(root->cmd)
// 			|| root->bracket_lvl < node->bracket_lvl))
// 	{
// 		node->left = root;
// 		return (node);
// 	}
// 	// If the node has the same bracket level as the root
// 	else if (root->bracket_lvl == node->bracket_lvl)
// 	{
// 		// Insert the node into the right subtree
// 		root->right = insert_node(root->right, node);
// 	}
// 	// If the node has a lower bracket level than the root
// 	else if (root->bracket_lvl > node->bracket_lvl)
// 	{
// 		// Insert the node as a new root, and make the current root the left child
// 		node->left = root;
// 		return (node);
// 	}
// 	return (root);
// }

int	main(void)
{
	t_app_data	app;
	t_token		*tokens;
	t_treenode	*root;
	t_treenode	*ast;

	app.input = ft_strdup("echo 1 && (echo 2 && echo 3) | cat -e");
	tokens = tokenize(&app);
	root = combine_cmds_args(tokens);
	// t_treenode *temp = root;
	// while (temp)
	// {
	// 	printf("cmd: %s, type: %d, args: %s, lvl: %d", temp->cmd, temp->cmd_type, temp->args, temp->bracket_lvl);
	// 	printf("\n----------\n");
	// 	temp = temp->left;
	// }
	ast = NULL;
	ast = build_ast(ast, root, 0);
	debug_printtree(ast, 0);
	return (0);
}

// build_ast
// 	- if no more nodes to process, return the ast
// 	- if the current node has the same bracket level as the ast
// 		- insert the node into the ast
// 		- build the ast with the left child
// 	- if the current node has a higher bracket level than the ast
// 		- build a subtree and insert it into the ast
// 		- build the ast with the current node
// 	- return the ast

// t_treenode	*build_ast(t_treenode *ast, t_treenode *lin_tree, int bracket_lvl)
// {
// 	if (!lin_tree)
// 		return (ast);
	
// }