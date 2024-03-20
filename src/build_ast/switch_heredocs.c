/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_heredocs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 11:06:32 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/20 14:28:13 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void		reverse(t_treenode **before_start, t_treenode **start,
					t_treenode **after_end);

// pseudo code minishell
// go through all lintrees
// check if lintree is heredoc
// if yes, add to heredocs array
// if no, continue
// go through heredocs array
// for each element, switch it with the lintree opposite to it: e.g. [1, 2, 3, 4, 5] -> [5, 4, 3, 2, 1]
// since there could be more than one heredoc chain, e.g. cat << EOF1 << EOF2, we need 
// a whileloop inside the while (current), that finishes the whole array, then goes to the left
// heredoc chain
// return the new lintrees

t_treenode	*switch_heredocs(t_treenode *lintree)
{
	t_treenode		*current;
	t_treenode		*after_end;

	current = lintree;
	while (current)
	{
		if (current && current->left && current->left->cmd_type == HEREDOC)
		{
			after_end = current->left;
			while (after_end && after_end->cmd_type == HEREDOC)
				after_end = after_end->left;
			reverse(&current, &current->left, &after_end);
			current = current->left;
			while (current && current->cmd_type == HEREDOC)
				current = current->left;
			continue ;
		}
		current = current->left;
	}
	return (lintree);
}

static void	reverse(t_treenode **before_start, t_treenode **start, t_treenode **after_end)
{
	t_treenode		*prev;
	t_treenode		*current;
	t_treenode		*next;

	current = *start;
	prev = *after_end;
	next = NULL;
	while (current && current->cmd_type == HEREDOC)
	{
		
		next = current->left;
		current->left = prev;
		prev = current;
		current = next;
	}
	(*before_start)->left = prev;
}
