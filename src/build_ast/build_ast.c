/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:48:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/06 17:12:49 by lzipp            ###   ########.fr       */
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

	if (!lin_tree)
		return (ast);
	if (lin_tree->bracket_lvl <= bracket_lvl)
	{
		new = new_treenode(lin_tree->cmd, lin_tree->args,
				lin_tree->bracket_lvl);
		ast = insert_node(ast, new);
		ast = build_ast(ast, lin_tree->left, lin_tree->bracket_lvl);
	}
	else if (lin_tree->bracket_lvl > bracket_lvl)
	{
		new = build_subtree(NULL, &lin_tree, lin_tree->bracket_lvl);
		ast = insert_node(ast, new);
		if (!lin_tree)
			return (ast);
		ast = build_ast(ast, lin_tree->left, lin_tree->bracket_lvl);
	}
	return (ast);
}

t_treenode	*build_subtree(t_treenode *sub, t_treenode **lin_tree,
				int bracket_lvl)
{
	t_treenode	*new;

	if (!(*lin_tree) || (*lin_tree)->bracket_lvl < bracket_lvl)
		return (sub);
	if ((*lin_tree)->bracket_lvl == bracket_lvl)
	{
		new = new_treenode((*lin_tree)->cmd, (*lin_tree)->args,
				(*lin_tree)->bracket_lvl);
		sub = insert_node(sub, new);
		*lin_tree = (*lin_tree)->left;
		if (!(*lin_tree))
			return (sub);
		sub = build_subtree(sub, lin_tree, (*lin_tree)->bracket_lvl);
	}
	else
	{
		new = build_subtree(NULL, lin_tree, (*lin_tree)->bracket_lvl);
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
	if (!root)
		return (node);
	if (node_is_operator(node->cmd) && priority(node->cmd, node->bracket_lvl)
		>= priority(root->cmd, root->bracket_lvl))
	{
		node->left = root;
		return (node);
	}
	root->right = insert_node(root->right, node);
	return (root);
}

// int	main(void)
// {
// 	t_app_data	app;
// 	t_token		*tokens;
// 	t_treenode	*root;
// 	t_treenode	*ast;

// 	app.input = ft_strdup("(echo 1 && (echo 2 || echo 3 && echo 4)) | cat -e");
// 	// app.input = ft_strdup("(echo 2 || echo 3) | cat -e");
// 	tokens = tokenize(&app);
// 	root = combine_cmds_args(tokens);
// 	ast = NULL;
// 	ast = build_ast(ast, root, 0);
// 	debug_printtree(ast, 0);
// 	return (0);
// }
