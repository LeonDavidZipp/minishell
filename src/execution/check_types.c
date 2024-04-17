/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_types.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:56:38 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/17 15:56:59 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_builtin(char *cmd, int exit_code, char **env_vars)
{
	char	*tmp;

	tmp = expand_and_remove(cmd, exit_code, env_vars, 0);
	if (ft_strcmp(tmp, "cd") == 0 || ft_strcmp(tmp, "pwd") == 0
		|| ft_strcmp(tmp, "echo") == 0 || ft_strcmp(tmp, "env") == 0
		|| ft_strcmp(tmp, "exit") == 0 || ft_strcmp(tmp, "export") == 0
		|| ft_strcmp(tmp, "unset") == 0)
		return (free(tmp), 1);
	free(tmp);
	return (0);
}

int	is_builtin_no_expand(char *cmd)
{
	char	*tmp;

	tmp = remove_quotes(cmd);
	if (ft_strcmp(tmp, "cd") == 0 || ft_strcmp(tmp, "pwd") == 0
		|| ft_strcmp(tmp, "echo") == 0 || ft_strcmp(tmp, "env") == 0
		|| ft_strcmp(tmp, "exit") == 0 || ft_strcmp(tmp, "export") == 0
		|| ft_strcmp(tmp, "unset") == 0)
		return (free(tmp), 1);
	free(tmp);
	return (0);
}

int	is_redir(t_tokentype type)
{
	if (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC)
		return (1);
	return (0);
}
