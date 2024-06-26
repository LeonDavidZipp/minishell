/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 14:18:00 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/16 16:24:32 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_treenode	*insert_node(t_treenode *root, t_treenode *node)
{
	if (!root)
		return (node);
	if (node_is_operator(node->cmd_type)
		&& priority(node->cmd, node->bracket_lvl)
		>= priority(root->cmd, root->bracket_lvl))
	{
		node->left = root;
		return (node);
	}
	root->right = insert_node(root->right, node);
	return (root);
}

bool	node_is_operator(t_tokentype type)
{
	return (type == PIPE || type == OR || type == AND || type == REDIR_OUT
		|| type == REDIR_APPEND || type == REDIR_IN || type == HEREDOC
		|| type == LEFT_BRACKET || type == RIGHT_BRACKET);
}

int	priority(char *cmd, int bracket_lvl)
{
	int		prio_1;
	int		prio_2;
	int		prio_3;

	prio_1 = 1 - 3 * bracket_lvl;
	prio_2 = 2 - 3 * bracket_lvl;
	prio_3 = 3 - 3 * bracket_lvl;
	if (ft_strcmp(cmd, ">") == 0 || ft_strcmp(cmd, ">>") == 0
		|| ft_strcmp(cmd, "<") == 0)
		return (prio_1);
	else if (ft_strcmp(cmd, "<<") == 0)
		return (-3 * bracket_lvl);
	else if (ft_strcmp(cmd, "|") == 0)
		return (prio_2);
	else if (ft_strcmp(cmd, "&&") == 0 || ft_strcmp(cmd, "||") == 0)
		return (prio_3);
	return (-3 * bracket_lvl);
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
		printf("\033[1;3%dmcontent: %s, args: %s, prio: %d, type: %d,"
			" br_lvl: %d, in_fd: %d, out_fd: %d \033[0m\n", 1 + tabs,
			root->cmd, root->args, priority(root->cmd, root->bracket_lvl),
			root->cmd_type, root->bracket_lvl, root->in_fd, root->out_fd);
		debug_print_tabs(tabs);
		printf("\033[1;3%dmleft:\n", 1 + tabs);
		debug_printtree(root->left, tabs + 1);
		debug_print_tabs(tabs);
		printf("\033[1;3%dmright:\033[0m\n", 1 + tabs);
		debug_printtree(root->right, tabs + 1);
	}
	else
	{
		debug_print_tabs(tabs);
		printf("\033[1;3%dmNULL\033[0m\n", 1 + tabs);
	}
}
