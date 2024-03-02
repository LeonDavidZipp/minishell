/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:28:17 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/02 14:44:47 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_tokentype	check_cmd_or_arg(char *content, t_tokentype prev_type);
// static t_tokentype	othercmd_or_arg(char *content, char *path);
// static t_tokentype	return_othercmd(char **paths, char *exec_path);

t_tokentype	token_type(char *content, t_tokentype prev_type)
{
	if (ft_strcmp(content, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(content, "&&") == 0)
		return (AND);
	else if (ft_strcmp(content, "||") == 0)
		return (OR);
	else if (ft_strcmp(content, ">") == 0)
		return (REDIR_OUT);
	else if (ft_strcmp(content, ">>") == 0)
		return (REDIR_APPEND);
	else if (ft_strcmp(content, "<") == 0)
		return (REDIR_IN);
	else if (ft_strcmp(content, "<<") == 0)
		return (HEREDOC);
	else if (ft_strcmp(content, "(") == 0)
		return (LEFT_BRACKET);
	else if (ft_strcmp(content, ")") == 0)
		return (RIGHT_BRACKET);
	else
		return (check_cmd_or_arg(content, prev_type));
}

static t_tokentype	check_cmd_or_arg(char *content, t_tokentype prev_type)
{
	if (ft_strcmp(content, "echo") == 0 || ft_strcmp(content, "cd") == 0
		|| ft_strcmp(content, "pwd") == 0 || ft_strcmp(content, "export") == 0
		|| ft_strcmp(content, "unset") == 0 || ft_strcmp(content, "env") == 0
		|| ft_strcmp(content, "exit") == 0)
		return (CMD);
	else if (prev_type == FIRST || prev_type == PIPE || prev_type == AND
		|| prev_type == OR || prev_type == LEFT_BRACKET)
		return (CMD);
	return (ARG);
}
