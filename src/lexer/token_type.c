/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:28:17 by intra             #+#    #+#             */
/*   Updated: 2024/02/29 15:32:32 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_tokentype	token_type_2(char *content, char *path);
static t_tokentype	token_type_3(char *content, char *path);

t_tokentype	token_type(char *content, char *path)
{
	if (ft_strcmp(content, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(content, "&&") == 0)
		return (AND);
	else if (ft_strcmp(content, "||") == 0)
		return (OR);
	else if (ft_strcmp(content, "*") == 0)
		return (WILDCARD);
	else if (ft_strcmp(content, ">") == 0)
		return (REDIR_OUT);
	else if (ft_strcmp(content, ">>") == 0)
		return (REDIR_APPEND);
	else if (ft_strcmp(content, "<") == 0)
		return (REDIR_IN);
	else if (ft_strcmp(content, "<<") == 0)
		return (HEREDOC);
	else if (ft_strcmp(content, "echo") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "cd") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "pwd") == 0)
		return (BUILTIN_CMD);
	else
		return (token_type_2(content, path));
}

static t_tokentype	token_type_2(char *content, char *path)
{
	if (ft_strcmp(content, "echo") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "cd") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "pwd") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "export") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "unset") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "env") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "exit") == 0)
		return (BUILTIN_CMD);
	else
		return (token_type_3(content, path));
}

static t_tokentype	token_type_3(char *content, char *path)
{
	char	**paths;
	int		i;

	paths = split_path(path);
	if (!paths)
		return (ARG);
	i = -1;
	while (paths[++i])
	{
		if (access(paths[i], X_OK) != -1 || ft_strcmp(content, "..") != 0
			|| ft_strcmp(content, ".") != 0)
		{
			ft_free_2d_arr((void **)paths);
			return (OTHER_CMD);
		}
		paths++;
	}
	ft_free_2d_arr((void **)paths);
	return (ARG);
}
