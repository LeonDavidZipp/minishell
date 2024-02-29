/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:28:17 by intra             #+#    #+#             */
/*   Updated: 2024/02/29 14:40:21 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_tokentype	token_type_2(char *content);

t_tokentype	token_type(char *content)
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
	else
		return (token_type_2(content));
}

static t_tokentype	token_type_2(char *content)
{
	char	*temp;

	if (ft_strcmp(content, "cd") == 0)
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
	{
		temp = ft_strjoin("/bin/", content);
		if (access(temp, X_OK) == -1 || ft_strcmp(content, "..") == 0
			|| ft_strcmp(content, ".") == 0)
			return (ARG);
		else
			return (OTHER_CMD);
		free(temp);
	}
}
