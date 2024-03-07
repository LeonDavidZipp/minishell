/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens_valid.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 22:01:18 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/07 12:13:29 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	check_and_or_pipe_redir_out_append(t_token *current);
static bool	check_redir_in_heredoc(t_token *current);
static bool	check_cd(t_token *current);
static bool	check_echo(t_token *current);

bool	check_tokens_valid(t_token *tokens)
{
	t_token		*current;
	t_tokentype	type;

	if (tokens->type == ARG || tokens->type == OR || tokens->type == AND
		|| tokens->type == PIPE)
		return (printf("%s: parse error near '%s'\n",
				NAME, tokens->next->content), false);
	current = tokens;
	while (current)
	{
		if (current->next)
			type = current->next->type;
		if (!check_and_or_pipe_redir_out_append(current))
			return (false);
		if (!check_redir_in_heredoc(current))
			return (false);
		if (!check_cd(current))
			return (false);
		if (!check_echo(current))
			return (false);
		current = current->next;
	}
	return (true);
}

static bool	check_and_or_pipe_redir_out_append(t_token *current)
{
	if (current->type == AND || current->type == OR || current->type == PIPE)
	{
		if (!current->next)
			return (printf("%s: parse error near '\\n'\n", NAME), false);
		else if (current->next->type == AND || current->next->type == OR
			|| current->next->type == PIPE)
			return (printf("%s: parse error near '%s'\n",
					NAME, current->next->content), false);
	}
	else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
	{
		if (!current->next)
			return (printf("%s: parse error near '\\n'\n", NAME), false);
		else if (current->next->type == AND || current->next->type == OR
			|| current->next->type == PIPE || current->next->type == REDIR_IN
			|| current->next->type == REDIR_OUT
			|| current->next->type == REDIR_APPEND
			|| current->next->type == HEREDOC)
			return (printf("%s: parse error near '%s'\n",
					NAME, current->next->content), false);
	}
	return (true);
}

static bool	check_redir_in_heredoc(t_token *current)
{
	if (current->type == REDIR_IN || current->type == HEREDOC)
	{
		if (!current->next)
			return (printf("%s: parse error near '\\n'\n", NAME), false);
		else if (current->next->type == AND || current->next->type == OR
			|| current->next->type == PIPE || current->next->type == REDIR_IN
			|| current->next->type == REDIR_OUT
			|| current->next->type == REDIR_APPEND
			|| current->next->type == HEREDOC)
			return (printf("%s: parse error near '%s'\n",
					NAME, current->next->content), false);
	}
	return (true);
}

static bool	check_cd(t_token *current)
{
	if (ft_strcmp(current->content, "cd") == 0)
	{
		if (current && current->next && current->next->next
			&& current->next->type != AND && current->next->type != OR
			&& current->next->type != PIPE && current->next->type != REDIR_OUT
			&& current->next->type != REDIR_APPEND
			&& current->next->type != REDIR_IN && current->next->type != HEREDOC
			&& current->next->next->type != AND
			&& current->next->next->type != OR
			&& current->next->next->type != PIPE
			&& current->next->next->type != REDIR_OUT
			&& current->next->next->type != REDIR_APPEND
			&& current->next->next->type != REDIR_IN
			&& current->next->next->type != HEREDOC)
			return (printf("cd: too many arguments\n"), false);
		else if (current->next && current->next && current->next->type != AND
			&& current->next->type != OR && current->next->type != PIPE
			&& current->next->type != REDIR_OUT
			&& current->next->type != REDIR_APPEND
			&& current->next->type != REDIR_IN
			&& current->next->type != HEREDOC)
			return (printf("cd: string not in pwd: '%s'\n",
					current->next->content), false);
	}
	return (true);
}

static bool	check_echo(t_token *current)
{
	if (ft_strcmp(current->content, "echo") == 0)
	{
		if (current && current->next && current->next->next
			&& (current->next->type == LEFT_BRACKET
				|| current->next->type == RIGHT_BRACKET))
		{
			printf("babash: syntax error near unexpected token \'%s\'\n",
				current->next->next->content);
			return (false);
		}
		else if (current->next && (current->next->type == LEFT_BRACKET
				|| current->next->type == RIGHT_BRACKET))
		{
			printf("babash: syntax error near unexpected token \'newline\'\n");
			return (false);
		}
	}
	return (true);
}

// int	main(void)
// {
// 	t_treenode	*nodes;
// 	t_treenode	*ast;

// 	nodes = malloc(sizeof(t_treenode));
// 	nodes->cmd = ft_strdup("echo");
// 	nodes->args = ft_strdup("hello");
// 	nodes->left = malloc(sizeof(t_treenode));
// 	nodes->left->cmd = ft_strdup(">");
// 	nodes->left->args = ft_strdup("file");
// 	nodes->left->left = NULL;
// 	nodes->left->right = NULL;
// 	nodes->right = NULL;
// 	ast = build_ast(nodes);
// 	if (check_nodes_valid(ast))
// 		printf("Valid\n");
// 	else
// 		printf("Invalid\n");
// 	free_treenodes(ast);
// 	return (0);
// }
