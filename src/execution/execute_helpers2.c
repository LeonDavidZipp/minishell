/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 18:30:23 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/17 18:41:55 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ambigious_redirect(char *str)
{
	int		i;
	bool	s_quote;
	bool	d_quote;

	i = 0;
	s_quote = false;
	d_quote = false;
	while (str[i])
	{
		handle_quotes(str[i], &s_quote, &d_quote);
		if (str[i] == ' ' && !s_quote && !d_quote)
			return (1);
		i++;
	}
	return (0);
}

int	handle_fds_dup2(t_treenode *ast)
{
	if (ast->in_fd != 0)
	{
		if (dup2(ast->in_fd, STDIN_FILENO) == -1)
			return (ft_fprintf(2, "%s: dup2 error: %s\n",
					NAME, strerror(errno)), 1);
		close(ast->in_fd);
	}
	if (ast->out_fd != 1)
	{
		if (dup2(ast->out_fd, STDOUT_FILENO) == -1)
			return (ft_fprintf(2, "%s: dup2 error: %s\n",
					NAME, strerror(errno)), 1);
		close(ast->out_fd);
	}
	return (0);
}

void	close_fds_loop(void)
{
	int	fd;

	fd = 3;
	while (fd < FD_SETSIZE)
	{
		if (fd != STDIN_FILENO && fd != STDOUT_FILENO)
			close(fd);
		fd++;
	}
}

void	handle_error(t_treenode *ast)
{
	if (ast->err_val == -1)
		ft_fprintf(2, "%s: %s: ambiguous redirect\n", NAME, ast->err);
	else
		ft_fprintf(2, "%s: %s: %s\n", NAME, ast->err, strerror(ast->err_val));
	if (ast->in_fd != 0)
		close(ast->in_fd);
	if (ast->out_fd != 1)
		close(ast->out_fd);
}
