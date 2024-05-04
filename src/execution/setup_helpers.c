/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 18:15:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/17 18:45:18 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	handle_in_fd(t_treenode *cmd_node, int fd);

t_treenode	*find_cmd_node(t_treenode *node)
{
	while (node->cmd_type != CMD)
	{
		if (node->right)
			node = node->right;
		else if (node->left)
			node = node->left;
		else
			break ;
	}
	return (node);
}

void	set_err(t_treenode *node, char *err, int val)
{
	t_treenode	*cmd_node;

	if (!node->left)
	{
		set_error_vars(node, err, val);
		return ;
	}
	cmd_node = find_cmd_node(node->left);
	if (cmd_node->cmd_type != CMD)
		set_error_vars(node, err, val);
	else
		set_error_vars(cmd_node, err, val);
	return ;
}

void	set_error_vars(t_treenode *node, char *err, int val)
{
	node->err_val = val;
	node->err = err;
}

void	set_fd(t_treenode *node, int fd, int flag)
{
	t_treenode	*cmd_node;

	if (!node->left)
	{
		close(fd);
		return ;
	}
	cmd_node = find_cmd_node(node->left);
	if (cmd_node && cmd_node->cmd_type != CMD)
		close(fd);
	if (cmd_node && flag == 1)
	{
		if (cmd_node->out_type == 1)
			close(fd);
		else
		{
			if (cmd_node->out_fd != STDOUT_FILENO)
				close(cmd_node->out_fd);
			cmd_node->out_fd = fd;
			cmd_node->out_type = 1;
		}
	}
	else
		handle_in_fd(cmd_node, fd);
}

static void	handle_in_fd(t_treenode *cmd_node, int fd)
{
	if (cmd_node->in_type == 1 || cmd_node->in_type == 2)
		close(fd);
	else
	{
		if (cmd_node->in_fd != STDIN_FILENO)
			close(cmd_node->in_fd);
		cmd_node->in_fd = fd;
		cmd_node->in_type = 1;
	}
}
