/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:48:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/11 15:56:31 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_treenode	*build_subtree(t_treenode *sub, t_treenode **lin_tree,
						int bracket_lvl);
static t_treenode	*insert_node(t_treenode *root, t_treenode *node);

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
		prev_bracket_lvl = (*lin_tree)->bracket_lvl;
		(*lin_tree) = (*lin_tree)->left;
		sub = build_subtree(sub, lin_tree, prev_bracket_lvl);
	}
	else
	{
		new = build_subtree(NULL, lin_tree, (*lin_tree)->bracket_lvl);
		sub = insert_node(sub, new);
		if (!(*lin_tree))
			return (sub);
		prev_bracket_lvl = (*lin_tree)->bracket_lvl;
		(*lin_tree) = (*lin_tree)->left;
		sub = build_subtree(sub, lin_tree, prev_bracket_lvl);
	}
	return (sub);
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

static t_treenode	*insert_node(t_treenode *root, t_treenode *node)
{
	if (!root)
		return (node);
	if (node_is_operator(node->cmd)
		// && node->cmd_type != REDIR_APPEND && node->cmd_type != REDIR_IN
		// && node->cmd_type != REDIR_OUT && node->cmd_type != HEREDOC
		&& priority(node->cmd, node->bracket_lvl)
		>= priority(root->cmd, root->bracket_lvl))
	{
		node->left = root;
		return (node);
	}
	root->right = insert_node(root->right, node);
	return (root);
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
