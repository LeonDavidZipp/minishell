/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 14:18:00 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/18 17:53:40 by lzipp            ###   ########.fr       */
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
		printf("\033[1;3%dmcontent: %s, args: %s, prio: %d, type: %d, br_lvl: %d \033[0m", 1 + tabs,
			root->cmd, root->args, priority(root->cmd, root->bracket_lvl),
			root->cmd_type, root->bracket_lvl);
		printf("\033[1;34min_fd: %d, \033[0m", root->in_fd);
		printf("\033[1;32mout_fd: %d, \033[0m", root->out_fd);
		printf("\033[1;33merr_val: %d\033[0m\n", root->err_val);
		debug_print_tabs(tabs);
		printf("\033[1;3%dmleft:\n", 1 + tabs);
		debug_printtree(root->left, tabs + 1);
		debug_print_tabs(tabs);
		printf("\033[1;3%dmright:\n", 1 + tabs);
		debug_printtree(root->right, tabs + 1);
	}
}