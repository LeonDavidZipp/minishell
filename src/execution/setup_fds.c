/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:44:39 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/18 14:41:23 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	setup_pipe(t_treenode *node, int *pipe_fd);
static int	setup_redir(t_treenode *node, t_app_data *app);
static int	handle_redir_out(t_treenode *node, char *tmp);
static int	handle_redir_in(t_treenode *node, char *tmp);

int	setup_fd(t_treenode *node, t_app_data *app, int *ret)
{
	int	pipe_fd[2];

	if (node->cmd_type == PIPE)
	{
		if (pipe(pipe_fd) == -1)
		{
			ft_fprintf(2, "%s: pipe error: %s\n", NAME, strerror(errno));
			*ret = 2;
			return (2);
		}
		else
			setup_pipe(node, pipe_fd);
	}
	if (is_redir(node->cmd_type))
		setup_redir(node, app);
	if (node->left)
		setup_fd(node->left, app, ret);
	if (node->right)
		setup_fd(node->right, app, ret);
	return (0);
}

static void	setup_pipe(t_treenode *node, int *pipe_fd)
{
	t_treenode	*cmd_node;

	cmd_node = find_cmd_node(node->left);
	if (cmd_node->cmd_type != CMD)
		close(pipe_fd[1]);
	else
	{
		cmd_node->out_fd = pipe_fd[1];
		cmd_node->pipe = true;
	}
	cmd_node = find_cmd_node(node->right);
	if (cmd_node->cmd_type != CMD)
		close(pipe_fd[0]);
	else
	{
		cmd_node->in_fd = pipe_fd[0];
		cmd_node->pipe = true;
	}
}

static int	setup_redir(t_treenode *node, t_app_data *app)
{
	char		*tmp;
	char		*tmp2;
	int			flags[2];

	flags[0] = 0;
	flags[1] = 0;
	if (!node->args)
		return (1);
	tmp2 = expand(node->args, app->last_exit_code, app->env_vars, flags);
	if (!tmp2)
		return (1);
	if (ambigious_redirect(tmp2))
		return (set_err(node, ft_strdup(node->args), -1), free(tmp2), 1);
	tmp = remove_quotes_in_place(tmp2);
	if (node->cmd_type == REDIR_IN)
		return (handle_redir_in(node, tmp));
	else if (node->cmd_type == HEREDOC)
		return (free(tmp), handle_heredoc(node, app));
	else if (node->cmd_type == REDIR_OUT || node->cmd_type == REDIR_APPEND)
		return (handle_redir_out(node, tmp));
	return (free(tmp), 0);
}

static int	handle_redir_out(t_treenode *node, char *tmp)
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

static int	handle_redir_in(t_treenode *node, char *tmp)
{
	int	tmp_fd;

	tmp_fd = open(tmp, O_RDONLY);
	if (tmp_fd == -1)
		return (set_err(node, tmp, errno), 1);
	else
		set_fd(node, tmp_fd, 2);
	return (0);
}
