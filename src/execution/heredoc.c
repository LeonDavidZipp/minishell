/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 17:57:09 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/24 16:35:36 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	heredoc_pipe_fail(t_treenode *node);
static int	read_input(char *delimiter, int write_fd,
				t_app_data *app, char *buf);
static void	init_read_input(int *i, int *exp_flags);
static int	handle_cmd_node(t_treenode *cmd_node, int tmp_fd);

int	handle_heredoc(t_treenode *node, t_app_data *app)
{
	int			tmp_fd;
	int			pipe_fd[2];
	t_treenode	*cmd_node;
	char		buf;

	if (pipe(pipe_fd) == -1)
	{
		heredoc_pipe_fail(node);
		return (1);
	}
	if (read_input(node->args, pipe_fd[1], app, &buf))
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	tmp_fd = pipe_fd[0];
	if (!node->left)
	{
		close(tmp_fd);
		return (1);
	}
	cmd_node = find_cmd_node(node->left);
	return (handle_cmd_node(cmd_node, tmp_fd));
}

static void	heredoc_pipe_fail(t_treenode *node)
{
	t_treenode	*cmd_node;

	if (!node->left)
	{
		set_error_vars(node, "heredoc pipe error", errno);
		return ;
	}
	cmd_node = find_cmd_node(node->left);
	if (cmd_node->cmd_type != CMD)
		set_error_vars(node, "heredoc pipe error", errno);
	else
		set_error_vars(cmd_node, "heredoc pipe error", errno);
}

static int	read_input(char *delimiter, int write_fd,
			t_app_data *app, char *buf)
{
	char	*line;
	char	*tmp;
	char	*new_del;
	int		i[2];
	int		exp_flags[2];

	init_read_input(i, exp_flags);
	new_del = process_delimiter(delimiter, i);
	line = get_next_line(0);
	while (line != NULL)
	{
		if (check_delimiter(&line, new_del) || read(0, &buf, 1) == 0)
			break ;
		tmp = exp_or_cpy(line, i[1], exp_flags, app);
		if (!tmp)
			return (free(line), free(new_del), 1);
		write(write_fd, tmp, ft_strlen(tmp));
		free(tmp);
		free(line);
		write(0, "> ", 2);
		line = get_next_line(0);
	}
	g_exit_signal = 0;
	return (free(line), free(new_del), close(write_fd), 0);
}

static void	init_read_input(int *i, int *exp_flags)
{
	i[0] = -1;
	i[1] = 1;
	exp_flags[0] = 1;
	exp_flags[1] = 0;
	g_exit_signal = 2;
}

static int	handle_cmd_node(t_treenode *cmd_node, int tmp_fd)
{
	if (cmd_node->cmd_type != CMD)
		return (close(tmp_fd), 1);
	if (cmd_node->in_type == 1)
		close(tmp_fd);
	else
	{
		cmd_node->in_fd = tmp_fd;
		cmd_node->in_type = 2;
	}
	return (0);
}
