/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_a_s_t.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:48:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/23 19:00:54 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_treenode	*new_node(t_token *token);

t_treenode	*build_a_s_t(t_token *tokens, int *depth)
{
	t_treenode	*root;

	if (tokens == NULL)
		return (NULL);
	root = new_node(tokens);
	if (tokens->next != NULL)
	{
		root->left = build_a_s_t(tokens->next, depth + 1);
		if (tokens->next->next != NULL)
			root->right = build_a_s_t(tokens->next->next, depth + 1);
	}
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
// 	char			**input;
// 	t_token			*tokens;
// 	t_treenode		*ast;

// 	input = ft_calloc(7, sizeof(char *));
// 	input[0] = ft_strdup("echo");
// 	input[1] = ft_strdup("hello");
// 	input[2] = ft_strdup("world");
// 	input[3] = ft_strdup("&&");
// 	input[4] = ft_strdup("ls");
// 	input[5] = ft_strdup("-la");
// 	tokens = tokenize(input);
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