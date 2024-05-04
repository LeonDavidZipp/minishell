/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:48:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/16 16:24:08 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_treenode	*build_subtree(t_treenode *sub, t_treenode **lin_tree,
						int bracket_lvl);
static void			adjust_values(t_treenode **lin_tree,
						int *prev_bracket_lvl);

t_treenode	*build_ast(t_treenode *ast, t_treenode *lin_tree, int bracket_lvl)
{
	t_treenode	*new;

	if (!lin_tree)
		return (ast);
	if (lin_tree->bracket_lvl <= bracket_lvl)
	{
		new = new_treenode(lin_tree->cmd, lin_tree->args,
				lin_tree->cmd_type, lin_tree->bracket_lvl);
		ast = insert_node(ast, new);
		ast = build_ast(ast, lin_tree->left, lin_tree->bracket_lvl);
	}
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

static t_treenode	*build_subtree(t_treenode *sub, t_treenode **lin_tree,
				int bracket_lvl)
{
	t_treenode	*new;
	int			prev_bracket_lvl;

	if (!(*lin_tree) || (*lin_tree)->bracket_lvl < bracket_lvl)
		return (sub);
	if ((*lin_tree)->bracket_lvl == bracket_lvl)
	{
		new = new_treenode((*lin_tree)->cmd, (*lin_tree)->args,
				(*lin_tree)->cmd_type, (*lin_tree)->bracket_lvl);
		sub = insert_node(sub, new);
		if (!(*lin_tree))
			return (sub);
		adjust_values(lin_tree, &prev_bracket_lvl);
		sub = build_subtree(sub, lin_tree, prev_bracket_lvl);
	}
	else
	{
		new = build_subtree(NULL, lin_tree, (*lin_tree)->bracket_lvl);
		sub = insert_node(sub, new);
		if (!(*lin_tree))
			return (sub);
		adjust_values(lin_tree, &prev_bracket_lvl);
		sub = build_subtree(sub, lin_tree, prev_bracket_lvl);
	}
	return (sub);
}

static void	adjust_values(t_treenode **lin_tree, int *prev_bracket_lvl)
{
	*prev_bracket_lvl = (*lin_tree)->bracket_lvl;
	(*lin_tree) = (*lin_tree)->left;
}

t_treenode	*new_treenode(char *cmd, char *args,
						t_tokentype cmd_type, int bracket_lvl)
{
	t_treenode		*node;

	node = malloc(sizeof(t_treenode));
	if (!node)
		return (NULL);
	if (bracket_lvl < 0)
		bracket_lvl = 0;
	node->cmd = ft_strdup(cmd);
	node->args = ft_strdup(args);
	node->cmd_type = cmd_type;
	node->bracket_lvl = bracket_lvl;
	node->left = NULL;
	node->right = NULL;
	node->in_fd = 0;
	node->out_fd = 1;
	node->err_val = 0;
	node->err = NULL;
	node->in_type = 0;
	node->out_type = 0;
	node->pipe = false;
	return (node);
}

void	free_treenodes(t_treenode *node)
{
	if (!node)
		return ;
	if (node->left)
		free_treenodes(node->left);
	if (node->right)
		free_treenodes(node->right);
	free(node->cmd);
	free(node->args);
	free(node->err);
	free(node);
}
