/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_a_s_t.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:48:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/28 14:10:45 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// static t_treenode	*new_node(t_token *token);

static bool	is_operator_2(char *cmd);
static int	operator_precedence(char *cmd);

t_treenode *create_node(char *cmd, char *args) {
	t_treenode *node = malloc(sizeof(t_treenode));
	node->cmd = cmd;
	node->args = args;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_treenode *insert_node(t_treenode *root, char *cmd, char *args) {
	// If the tree is empty, assign a new node address to root
	if (root == NULL) {
		return create_node(cmd, args);
	}

	// If the command is an operator and has higher precedence than the root
	if (is_operator_2(cmd) && operator_precedence(cmd) > operator_precedence(root->cmd)) {
		t_treenode *node = create_node(cmd, args);
		node->left = root;
		return node;
	}

	// Else, recursively insert the node into the right subtree
	root->right = insert_node(root->right, cmd, args);
	return root;
	}

static bool	is_operator_2(char *cmd)
{
	return (ft_strcmp(cmd, "|") == 0 || ft_strcmp(cmd, "||") == 0
		|| ft_strcmp(cmd, "&&") == 0 || ft_strcmp(cmd, ">") == 0
		|| ft_strcmp(cmd, ">>") == 0 || ft_strcmp(cmd, "<") == 0
		|| ft_strcmp(cmd, "<<") == 0 || ft_strcmp(cmd, ";") == 0);
}

static int	operator_precedence(char *cmd)
{
	if (ft_strcmp(cmd, "|") == 0)
		return 1;
	if (ft_strcmp(cmd, "||") == 0)
		return 2;
	if (ft_strcmp(cmd, "&&") == 0)
		return 3;
	if (ft_strcmp(cmd, ">") == 0 || ft_strcmp(cmd, ">>") == 0
		|| ft_strcmp(cmd, "<") == 0 || ft_strcmp(cmd, "<<") == 0)
		return 4;
	if (ft_strcmp(cmd, ";") == 0)
		return 5;
	return 0;
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

// int	main(void)
// {
// 	t_app_data		app;
// 	t_token			*tokens;
// 	t_treenode		*ast;

// 	app.input = ft_strdup("command1 | command 2");
// 	tokens = tokenize(&app);
// 	printf("tokens:\n");
// 	t_token	*temp = tokens;
// 	while (temp)
// 	{
// 		printf("content: %s - type: %d\n", temp->content, temp->type);
// 		temp = temp->next;
// 	}
// 	printf("------------------------\n");
// 	ast = build_a_s_t(tokens, 0);
// 	debug_printtree(ast, 0);
// }
