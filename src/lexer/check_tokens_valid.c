/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens_valid.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 22:01:18 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/02 14:08:59 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	check_first_token(t_token *token);

bool	check_tokens_valid(t_token *tokens)
{
	t_token		*current;
	t_tokentype	type;

	if (!check_first_token(tokens))
		return (false);
	current = tokens;
	while (current)
	{
		if (current->next)
			type = current->next->type;
		if (current->type == AND || current->type == OR
			|| current->type == PIPE)
		{
			if (!current->next)
				return (printf("%s: parse error near '\\n'\n", NAME), false);
			else if (type == AND || type == OR || type == PIPE)
				return (printf("%s: parse error near '%s'\n",
						NAME, current->next->content), false);
		}
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (!current->next)
				return (printf("%s: parse error near '\\n'\n", NAME), false);
			else if (type == AND || type == OR || type == PIPE
				|| type == REDIR_IN || type == REDIR_OUT || type == REDIR_APPEND
				|| type == HEREDOC)
				return (printf("%s: parse error near '%s'\n",
						NAME, current->next->content), false);
		}
		if (current->type == REDIR_IN || current->type == HEREDOC)
		{
			if (!current->next)
				return (printf("%s: parse error near '\\n'\n", NAME), false);
			else if (type == AND || type == OR || type == PIPE
				|| type == REDIR_IN || type == REDIR_OUT || type == REDIR_APPEND
				|| type == HEREDOC)
				return (printf("%s: parse error near '%s'\n",
						NAME, current->next->content), false);
		}
		if (ft_strcmp(current->content, "cd") == 0)
		{
			if (current->next && current->next->next
				&& current->next->next->next
				&& current->next->next->type != AND
				&& current->next->next->type != OR
				&& current->next->next->type != PIPE
				&& current->next->next->type != REDIR_OUT
				&& current->next->next->type != REDIR_APPEND
				&& current->next->next->type != REDIR_IN
				&& current->next->next->type != HEREDOC
				&& current->next->next->next->type != AND
				&& current->next->next->next->type != OR
				&& current->next->next->next->type != PIPE
				&& current->next->next->next->type != REDIR_OUT
				&& current->next->next->next->type != REDIR_APPEND
				&& current->next->next->next->type != REDIR_IN
				&& current->next->next->next->type != HEREDOC)
				return (printf("cd: too many arguments\n"), false);
			else if (current->next && current->next->next
				&& current->next->next->type != AND
				&& current->next->next->type != OR
				&& current->next->next->type != PIPE
				&& current->next->next->type != REDIR_OUT
				&& current->next->next->type != REDIR_APPEND
				&& current->next->next->type != REDIR_IN
				&& current->next->next->type != HEREDOC)
				return (printf("cd: string not in pwd: '%s'\n",
						current->next->content), false);
		}
		if (ft_strcmp(current->content, "echo") == 0)
		{
			if (current->next && current->next->next
				&& (current->next->type == LEFT_BRACKET
				|| current->next->type == RIGHT_BRACKET))
				return (printf("babash: syntax error near unexpected token \'%s\'\n",
						current->next->next->content), false);
			else if (current->next && (current->next->type == LEFT_BRACKET
				|| current->next->type == RIGHT_BRACKET))
				return (printf("babash: syntax error near unexpected token \'newline\'\n"),
						false);
		}
		current = current->next;
	}
	return (true);
}

bool	check_first_token(t_token *token)
{
	if (token->type == ARG
		|| token->type == OR
		|| token->type == AND
		|| token->type == PIPE)
		return (printf("%s: parse error near '%s'\n",
				NAME, token->next->content), false);
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
