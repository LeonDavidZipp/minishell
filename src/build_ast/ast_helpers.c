/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 14:18:00 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/19 10:51:26 by lzipp            ###   ########.fr       */
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
	int		lowest_prio;
	int		low_prio;
	int		mid_prio;
	int		high_prio;
	int		highest_prio;

	lowest_prio = 0 - 4 * bracket_lvl;
	low_prio = 1 - 4 * bracket_lvl;
	mid_prio = 2 - 4 * bracket_lvl;
	high_prio = 3 - 4 * bracket_lvl;
	highest_prio = 4 - 4 * bracket_lvl;
	if (ft_strcmp(cmd, ">") == 0 || ft_strcmp(cmd, ">>") == 0
		|| ft_strcmp(cmd, "<") == 0)
		return (mid_prio);
	else if (ft_strcmp(cmd, "<<") == 0)
		return (lowest_prio);
	else if (ft_strcmp(cmd, "|") == 0)
		return (high_prio);
	else if (ft_strcmp(cmd, "&&") == 0 || ft_strcmp(cmd, "||") == 0)
		return (highest_prio);
	return (low_prio);
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
