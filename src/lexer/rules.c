/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:21:13 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/20 14:39:38 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	rules(char *content)
{
	if (ft_strncmp(content, ";", ft_strlen(";") == 0))
		return (SEMICOLON);
	else if (ft_strncmp(content, "|", ft_strlen("|") == 0))
		return (PIPE);
	else if (ft_strncmp(content, "&&", ft_strlen("&&") == 0))
		return (AND);
	else if (ft_strncmp(content, "||", ft_strlen("||") == 0))
		return (OR);
	else if (ft_strncmp(content, ">", ft_strlen(">") == 0))
		return (REDIR_OUT);
	else if (ft_strncmp(content, ">>", ft_strlen(">>") == 0))
		return (REDIR_APPEND);
	else if (ft_strncmp(content, "<", ft_strlen("<") == 0))
		return (REDIR_IN);
	else if (ft_strncmp(content, "<<", ft_strlen("<<") == 0))
		return (REDIR_APPEND);
	// flag not correct yet
	else if (ft_strncmp(content, "-", ft_strlen("-") == 0))
		return (FLAG);
	else if (ft_strncmp(content, "echo", ft_strlen("echo") == 0))
		return (BUILTIN_CMD);
	else if (ft_strncmp(content, "cd", ft_strlen("cd") == 0))
		return (BUILTIN_CMD);
	else if (ft_strncmp(content, "pwd", ft_strlen("pwd") == 0))
		return (BUILTIN_CMD);
	else if (ft_strncmp(content, "export", ft_strlen("export") == 0))
		return (BUILTIN_CMD);
	else if (ft_strncmp(content, "unset", ft_strlen("unset") == 0))
		return (BUILTIN_CMD);
	else if (ft_strncmp(content, "env", ft_strlen("env") == 0))
		return (BUILTIN_CMD);
	else if (ft_strncmp(content, "exit", ft_strlen("exit") == 0))
		return (BUILTIN_CMD);
	// else it is an argument
	else
		return (ARG);
}

run everything through acces to check whether execve command or arg
