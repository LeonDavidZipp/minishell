/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:48:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/28 14:58:13 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool			node_is_operator(char *cmd);
static int			priority(char *cmd);
static t_treenode	*new_treenode(char *cmd, char *args);
static t_treenode	*insert_node(t_treenode *root, char *cmd, char *args);

static t_treenode	*new_treenode(char *cmd, char *args)
{
	t_treenode		*node;

	node = malloc(sizeof(t_treenode));
	node->cmd = cmd;
	node->args = args;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

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
	free_treenodes(lin_tree);
	return (ast);
}

static t_treenode	*insert_node(t_treenode *root, char *cmd, char *args)
{
	t_treenode	*node;

	// If the tree is empty, assign a new node address to root
	if (root == NULL)
		return (new_treenode(cmd, args));
	// If the command is an operator and has higher precedence than the root
	if (node_is_operator(cmd) && priority(cmd) > priority(root->cmd))
	{
		node = (new_treenode(cmd, args));
		node->left = root;
		return (node);
	}
	// Else, recursively insert the node into the right subtree
	root->right = insert_node(root->right, cmd, args);
	return (root);
}

static bool	node_is_operator(char *cmd)
{
	return (ft_strcmp(cmd, "|") == 0 || ft_strcmp(cmd, "||") == 0
		|| ft_strcmp(cmd, "&&") == 0 || ft_strcmp(cmd, ">") == 0
		|| ft_strcmp(cmd, ">>") == 0 || ft_strcmp(cmd, "<") == 0
		|| ft_strcmp(cmd, "<<") == 0);
}

static int	priority(char *cmd)
{
	if (ft_strcmp(cmd, ">") == 0 || ft_strcmp(cmd, ">>") == 0
		|| ft_strcmp(cmd, "<") == 0 || ft_strcmp(cmd, "<<") == 0)
		return (1);
	else if (ft_strcmp(cmd, "|") == 0)
		return (2);
	else if (ft_strcmp(cmd, "&&") == 0)
		return (3);
	else if (ft_strcmp(cmd, "||") == 0)
		return (4);
	return (0);
}

// static t_treenode	*new_treenode(t_token *token)
// {
// 	t_treenode	*node;

// 	node = (t_treenode *)malloc(sizeof(t_treenode));
// 	if (!node)
// 		return (NULL);
// 	node->cmd = ft_strdup(token->content);
// 	node-> = token->type;
// 	node->left = NULL;
// 	node->right = NULL;
// 	return (node);
// }

// static void	debug_print_tabs(int tabs)
// {
// 	while (tabs--)
// 		printf("\t");
// }

// static void	debug_printtree(t_treenode *root, int tabs)
// {
// 	if (root)
// 	{
// 		debug_print_tabs(tabs);
// 		printf("content: %s, type: %d\n", root->content, root->type);
// 		debug_print_tabs(tabs);
// 		printf("left:\n");
// 		debug_printtree(root->left, tabs + 1);
// 		debug_print_tabs(tabs);
// 		printf("right:\n");
// 		debug_printtree(root->right, tabs + 1);
// 	}
// 	else
// 	{
// 		debug_print_tabs(tabs);
// 		printf("--is empty--\n");
// 	}
// }

int	main(void)
{
	t_app_data	app;
	t_token		*tokens;
	t_treenode	*root;

	app.input = ft_strdup("echo -n hello how are you && cd .. * .");
	tokens = tokenize(&app);
	root = combine_tokens(tokens);
	root = build_ast(root);
	// debug_printtree(root, 0);
	return (0);
}
