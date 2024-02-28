/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_s_t_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 14:18:00 by intra             #+#    #+#             */
/*   Updated: 2024/02/28 14:19:11 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_treenodes(t_treenode *node)
{
	if (node->left)
		free_treenodes(node->left);
	if (node->right)
		free_treenodes(node->right);
	free(node->cmd);
	free(node->args);
	free(node);
}
