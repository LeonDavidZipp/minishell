/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:44:39 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/17 12:45:49 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_redir_out(t_treenode *node, char *tmp);
int	handle_redir_in(t_treenode *node, char *tmp);

int	setup_redir(t_treenode *node, t_app_data *app)
{
	char		*tmp;
	int 		flags[2];

	flags[0] = 0;
	flags[1] = 0;
	if (!node->args)
		return (1);
	char *tmp2 = expand(node->args, app->last_exit_code, app->env_vars, flags);
	if (!tmp2)
		return (1);
	if (ambigious_redirect(tmp2))
		return (set_err(node, ft_strdup(node->args), -1), free(tmp2), 1);
	tmp = remove_quotes_in_place(tmp2);
	if (node->cmd_type == REDIR_IN)
		return (handle_redir_in(node, tmp));
	else if (node->cmd_type == HEREDOC && g_exit_signal != 2)
		return (free(tmp), handle_heredoc(node, app));
	else if (node->cmd_type == REDIR_OUT || node->cmd_type == REDIR_APPEND)
		return (handle_redir_out(node, tmp));
	return (free(tmp), 0);
}

int	handle_redir_out(t_treenode *node, char *tmp)
{
	int	tmp_fd;

	if (node->cmd_type == REDIR_OUT)
			tmp_fd = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		tmp_fd = open(tmp, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (tmp_fd == -1)
		return (set_err(node, tmp, errno), 1);
	else
		set_fd(node, tmp_fd, 1);
	return (0);
}

int	handle_redir_in(t_treenode *node, char *tmp)
{
	int	tmp_fd;

	tmp_fd = open(tmp, O_RDONLY);
	if (tmp_fd == -1)
		return (set_err(node, tmp, errno), 1);
	else
		set_fd(node, tmp_fd, 2);
	return (0);
}

