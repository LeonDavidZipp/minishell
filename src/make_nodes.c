/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:48:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/14 14:51:20 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_treenode	*new_node(t_input *input)
{
	t_treenode	*node;

	node = (t_treenode *)malloc(sizeof(t_treenode));
	if (!node)
		return (NULL);
	node->content = input->content;
	node->type = input->type;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	debug_print_tabs(int tabs)
{
	while (tabs--)
		printf("\t");
}

void	debug_printtree(t_treenode *root, int tabs)
{
	if (root)
	{
		debug_print_tabs(tabs);
		printf("content: %s, type: %d\n", root->content, root->type);
		debug_print_tabs(tabs);
		printf("left:\n");
		debug_printtree(root->left, tabs + 1);
		debug_print_tabs(tabs);
		printf("right:\n");
		debug_printtree(root->right, tabs + 1);
	}
	else
	{
		debug_print_tabs(tabs);
		printf("--is empty--\n");
	}
}

/*
int	main(void)
{
	t_input		inputs[7];
	t_treenode	*root;
	inputs[0].content = "echo";
	inputs[0].type = BUILTIN_CMD;
	inputs[1].content = "hello";
	inputs[1].type = ARG;
	inputs[2].content = ">";
	inputs[2].type = REDIR_OUT;
	inputs[3].content = "file";
	inputs[3].type = ARG;
	inputs[4].content = "|";
	inputs[4].type = PIPE;
	inputs[5].content = "cat";
	inputs[5].type = BUILTIN_CMD;
	inputs[6].content = NULL;
	inputs[6].type = 0;
	root = (t_treenode *)new_node(&inputs[0]);
	root->left = (t_treenode *)new_node(&inputs[1]);
	root->right = (t_treenode *)new_node(&inputs[2]);
	root->right->left = (t_treenode *)new_node(&inputs[3]);
	root->right->right = (t_treenode *)new_node(&inputs[4]);
	root->right->right->left = (t_treenode *)new_node(&inputs[5]);
	debug_printtree(root, 0);
	return (0);
}
*/
