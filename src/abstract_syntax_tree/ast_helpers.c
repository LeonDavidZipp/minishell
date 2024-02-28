/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 14:18:00 by intra             #+#    #+#             */
/*   Updated: 2024/02/28 18:57:24 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	node_is_operator(char *cmd)
{
	return (ft_strcmp(cmd, "|") == 0 || ft_strcmp(cmd, "||") == 0
		|| ft_strcmp(cmd, "&&") == 0 || ft_strcmp(cmd, ">") == 0
		|| ft_strcmp(cmd, ">>") == 0 || ft_strcmp(cmd, "<") == 0
		|| ft_strcmp(cmd, "<<") == 0);
}

int	priority(char *cmd)
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

static void	debug_print_tabs(int tabs)
{
	while (tabs--)
		printf("\t");
}

void	debug_printtree(t_treenode *root, int tabs)
{
	if (root)
	{
		debug_print_tabs(tabs);
		printf("content: %s, args: %s\n", root->cmd, root->args);
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
