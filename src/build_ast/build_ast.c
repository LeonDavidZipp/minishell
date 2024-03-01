/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:48:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 11:34:20 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_treenode	*new_treenode(char *cmd, char *args);
static t_treenode	*insert_node(t_treenode *root, char *cmd, char *args);

t_treenode	*build_ast(t_treenode *lin_tree)
{
	t_treenode	*ast;
	t_treenode	*temp;

	ast = NULL;
	temp = lin_tree;
	while (temp)
	{
		ast = insert_node(ast, temp->cmd, temp->args);
		temp = temp->left;
	}
	// free_treenodes(lin_tree);
	return (ast);
}

static t_treenode	*new_treenode(char *cmd, char *args)
{
	t_treenode		*node;

	node = malloc(sizeof(t_treenode));
	node->cmd = ft_strdup(cmd);
	node->args = ft_strdup(args);
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

static t_treenode	*insert_node(t_treenode *root, char *cmd, char *args)
{
	t_treenode	*node;

	// If the tree is empty, assign a new node address to root
	// printf("inserting node with cmd: %s and args: %s\n", cmd, args);
	if (root == NULL)
		return (new_treenode(cmd, args));
	// If the command is an operator and has higher precedence than the root
	if (node_is_operator(cmd) && priority(cmd) > priority(root->cmd))
	{
		node = new_treenode(cmd, args);
		node->left = root;
		return (node);
	}
	// Else, recursively insert the node into the right subtree
	root->right = insert_node(root->right, cmd, args);
	return (root);
}

// int	main(void)
// {
// 	t_app_data	app;
// 	t_token		*tokens;
// 	t_treenode	*root;
// 	t_treenode	*ast;

// 	app.input = ft_strdup("echo -n hello how are you && cd .. 
// || echo hi || echo hello u");
// 	tokens = tokenize(&app);
// 	root = combine_cmds_args(tokens);
// 	ast = build_ast(root);
// 	debug_printtree(ast, 0);
// 	return (0);
// }
