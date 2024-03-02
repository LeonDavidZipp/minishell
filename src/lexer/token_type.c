/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 20:30:55 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 20:30:59 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_tokentype	token_type_2(char *content, char *path);
static t_tokentype	othercmd_or_arg(char *content, char *path);
static t_tokentype	return_othercmd(char **paths, char *exec_path);

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
	if (ft_strcmp(content, "(") == 0)
		return (LEFT_BRACKET);
	else if (ft_strcmp(content, ")") == 0)
		return (RIGHT_BRACKET);
	else if (ft_strcmp(content, "echo") == 0)
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
		return (othercmd_or_arg(content, path));
}

static t_tokentype	othercmd_or_arg(char *content, char *path)
{
	struct stat	s;
	char		**paths;
	char		*exec_path;
	int			i;

	if (access(content, X_OK) != -1 && stat(content, &s) == 0
		&& S_ISREG(s.st_mode) && ft_strcmp(content, "..") != 0
		&& ft_strcmp(content, ".") != 0)
		return (OTHER_CMD);
	paths = split_path(path);
	if (!paths)
		return (ARG);
	i = -1;
	while (paths[++i])
	{
		exec_path = ft_strjoin(paths[i], content);
		if (access(exec_path, X_OK) != -1 && stat(exec_path, &s) == 0
			&& S_ISREG(s.st_mode) && ft_strcmp(content, "..") != 0
			&& ft_strcmp(content, ".") != 0)
			return (return_othercmd(paths, exec_path));
		free(exec_path);
	}
	ft_free_2d_arr((void **)paths);
	return (ARG);
}

static t_tokentype	return_othercmd(char **paths, char *exec_path)
{
	ft_free_2d_arr((void **)paths);
	free(exec_path);
	return (OTHER_CMD);
}
