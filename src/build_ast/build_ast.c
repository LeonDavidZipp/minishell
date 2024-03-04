/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:48:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/04 11:51:48 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// static t_treenode	*build_subtree(t_treenode **temp);
static t_treenode	*new_treenode(char *cmd, char *args);
static t_treenode	*insert_node(t_treenode *root, t_treenode *node);

// t_treenode	*build_ast(t_treenode *ast, t_treenode **lin_tree,
// 						int old_bracket_lvl)
// {
// 	t_treenode	*ast = NULL;
// 	t_treenode	*new;

// 	if (!*lin_tree)
// 		return (NULL);
// 	if ((*lin_tree)->bracket_lvl > old_bracket_lvl)
// 	{
// 		old_bracket_lvl = (*lin_tree)->bracket_lvl;
// 		ast = insert_node(ast, build_ast(lin_tree, old_bracket_lvl));
// 	}
// 	else
// 	{
// 		old_bracket_lvl = (*lin_tree)->bracket_lvl;
// 		new = new_treenode((*lin_tree)->cmd, (*lin_tree)->args);
// 		ast = insert_node(ast, new);
// 		*lin_tree = (*lin_tree)->left;
// 	}
// 	ast->right = build_ast(lin_tree, old_bracket_lvl);
// 	return (ast);
// }

// t_treenode	*build_ast(t_treenode *lin_tree)
// {
// 	t_treenode	*ast;
// 	t_treenode	*temp;
// 	t_treenode	*new;
// 	int			old_bracket_lvl;

// 	ast = NULL;
// 	temp = lin_tree;
// 	old_bracket_lvl = 0;
// 	while (temp)
// 	{
// 		if (temp->bracket_lvl > old_bracket_lvl)
// 		{
// 			old_bracket_lvl = temp->bracket_lvl;
// 			ast = insert_node(ast, build_subtree(&temp, old_bracket_lvl));
// 		}
// 		else
// 		{
// 			old_bracket_lvl = temp->bracket_lvl;
// 			new = new_treenode(temp->cmd, temp->args);
// 			ast = insert_node(ast, new);
// 			temp = temp->left;
// 		}
// 	}
// 	// free_treenodes(lin_tree);
// 	return (ast);
// }

// static t_treenode	*build_subtree(t_treenode **temp, int old_bracket_lvl)
// {
// 	t_treenode	*root;
// 	t_treenode	*new_node;

// 	root = NULL;
// 	while (*temp && (*temp)->bracket_lvl)
// 	{
// 		if ((*temp)->bracket_lvl > old_bracket_lvl)
// 		{
// 			root = insert_node(root, build_subtree(&(*temp)->left,
// 						(*temp)->bracket_lvl));
// 		}
// 		else if ((*temp)->bracket_lvl < old_bracket_lvl)
// 			return (root);
// 		else
// 		{
// 			root = insert_node(root, *temp);
// 			*temp = (*temp)->left;
// 		}
// 	}
// 	return (root);
// }

t_treenode	*build_ast(t_treenode *ast, t_treenode *lin_tree, int bracket_lvl)
{
	t_treenode	*new;

	if (!lin_tree)
		return (ast);
	if (lin_tree->bracket_lvl == bracket_lvl)
	{
		new = new_treenode(lin_tree->cmd, lin_tree->args, lin_tree->bracket_lvl);
		ast = insert_node(ast, new);
	}
	else if (lin_tree->bracket_lvl > bracket_lvl)
	{
		ast = build_ast(ast, lin_tree->left, lin_tree->bracket_lvl);
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

// static t_treenode	*insert_node(t_treenode *root, char *cmd, char *args)
// {
// 	t_treenode	*node;

// 	// If the tree is empty, assign a new node address to root
// 	// printf("inserting node with cmd: %s and args: %s\n", cmd, args);
// 	if (root == NULL)
// 		return (new_treenode(cmd, args));
// 	// If the command is an operator and has higher precedence than the root
// 	if (node_is_operator(cmd) && priority(cmd) > priority(root->cmd))
// 	{
// 		node = new_treenode(cmd, args);
// 		node->left = root;
// 		return (node);
// 	}
// 	// Else, recursively insert the node into the right subtree
// 	root->right = insert_node(root->right, cmd, args);
// 	return (root);
// }

static t_treenode	*insert_node(t_treenode *root, t_treenode *node)
{
	// If the tree is empty, assign a new node address to root
	if (!root)
		return (node);
	if (!node)
		return (root);
	// If the command is an operator and has higher precedence than the root
	if (node_is_operator(node->cmd) && priority(node->cmd) > priority(root->cmd))
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

	app.input = ft_strdup("echo -n hello how are you && cd .. 
|| echo hi || echo hello u");
	tokens = tokenize(&app);
	root = combine_cmds_args(tokens);
	ast = build_ast(root);
	debug_printtree(ast, 0);
	return (0);
}
