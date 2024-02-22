/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:21:13 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/22 15:05:13 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	rules(char *content)
{
	// obvious rulings: everything that can be directly assigned to a token type
	// ############################################################
	if (ft_strncmp(content, "|", ft_strlen("|") == 0))
		return (PIPE);
	else if (ft_strncmp(content, "'", ft_strlen("'") == 0))
		return (SINGLE_QUOTE);
	else if (ft_strncmp(content, "\"", ft_strlen("\"") == 0))
		return (DOUBLE_QUOTE);
	else if (ft_strncmp(content, "&&", ft_strlen("&&") == 0))
		return (AND);
	else if (ft_strncmp(content, "||", ft_strlen("||") == 0))
		return (OR);
	else if (ft_strncmp(content, "*", ft_strlen("*") == 0))
		return (WILDCARD);
	else if (ft_strncmp(content, ">", ft_strlen(">") == 0))
		return (REDIR_OUT);
	else if (ft_strncmp(content, ">>", ft_strlen(">>") == 0))
		return (REDIR_INPUT);
	else if (ft_strncmp(content, "<", ft_strlen("<") == 0))
		return (REDIR_IN);
	else if (ft_strncmp(content, "<<", ft_strlen("<<") == 0))
		return (HEREDOC);
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
	else if (content && content[0] == '-')
		return (FLAG);
	// ############################################################
	// everything below here is either an execve command or an argument
	// if its an execve command, access will return 0
	else if (access(content, F_OK) != -1)
	{
		// file exists
		if (access(content, X_OK) != -1)
		{
			// file is executable
			return (OTHER_CMD);
		} else {
			// file is not executable
			return (ARG);
		}
	} 
}
