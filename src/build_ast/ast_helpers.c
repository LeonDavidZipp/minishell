/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 14:18:00 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/08 18:36:37 by cgerling         ###   ########.fr       */
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
		printf("\033[1;3%dmcontent: %s, args: %s, prio: %d, type: %d, br_lvl: %d, in_fd: %d, out_fd: %d \033[0m\n", 1 + tabs,
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
