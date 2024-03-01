/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens_valid.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 22:01:18 by intra             #+#    #+#             */
/*   Updated: 2024/03/01 08:28:03 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	check_first_token(t_token *token);

bool	check_tokens_valid(t_token *tokens)
{
	t_tokentype	type;

	if (!check_first_token(tokens))
		return (false);
	while (tokens)
	{
		// && and ||
		if (tokens->next)
			type = tokens->next->type;
		if (tokens->type == AND || tokens->type == OR || tokens->type == PIPE)
		{
			if (!tokens->next)
				return (printf("%s: parse error near `\\n'\n", NAME), false);
			else if (type == AND || type == OR || type == PIPE)
				return (printf("%s: parse error near `%s'\n",
						NAME, tokens->next->content), false);
		}
		// > and >>
		if (tokens->type == REDIR_OUT || tokens->type == REDIR_APPEND)
		{
			if (!tokens->next)
				return (printf("%s: parse error near `\\n'\n", NAME), false);
			else if (type == AND || type == OR || type == PIPE
				|| type == REDIR_IN || type == REDIR_OUT || type == REDIR_APPEND
				|| type == HEREDOC || type == WILDCARD)
				return (printf("%s: parse error near `%s'\n",
						NAME, tokens->next->content), false);
		}
		// < and <<
		if (tokens->type == REDIR_IN || tokens->type == HEREDOC)
		{
			if (!tokens->next)
				return (printf("%s: parse error near `\\n'\n", NAME), false);
			else if (type == AND || type == OR || type == PIPE
				|| type == REDIR_IN || type == REDIR_OUT || type == REDIR_APPEND
				|| type == HEREDOC || type == WILDCARD)
				return (printf("%s: parse error near `%s'\n",
						NAME, tokens->next->content), false);
		}
		if (ft_strcmp(tokens->content, "cd") == 0)
		{
			if (tokens->next && tokens->next->next
				&& tokens->next->next->next
				&& tokens->next->next->type != AND
				&& tokens->next->next->type != OR
				&& tokens->next->next->type != PIPE
				&& tokens->next->next->type != REDIR_OUT
				&& tokens->next->next->type != REDIR_APPEND
				&& tokens->next->next->type != REDIR_IN
				&& tokens->next->next->type != HEREDOC
				&& tokens->next->next->type != WILDCARD
				&& tokens->next->next->next->type != AND
				&& tokens->next->next->next->type != OR
				&& tokens->next->next->next->type != PIPE
				&& tokens->next->next->next->type != REDIR_OUT
				&& tokens->next->next->next->type != REDIR_APPEND
				&& tokens->next->next->next->type != REDIR_IN
				&& tokens->next->next->next->type != HEREDOC
				&& tokens->next->next->next->type != WILDCARD)
				return (printf("cd: too many arguments\n"), false);
			else if (tokens->next && tokens->next->next
				&& tokens->next->next->type != AND
				&& tokens->next->next->type != OR
				&& tokens->next->next->type != PIPE
				&& tokens->next->next->type != REDIR_OUT
				&& tokens->next->next->type != REDIR_APPEND
				&& tokens->next->next->type != REDIR_IN
				&& tokens->next->next->type != HEREDOC
				&& tokens->next->next->type != WILDCARD)
				return (printf("cd: string not in pwd: `%s'\n",
						tokens->next->content), false);
		}
		// echo, cd, pwd, export, unset, env
	}
	return (true);
}

bool	check_first_token(t_token *token)
{
	if (token->type == ARG
		|| token->type == OR
		|| token->type == AND
		|| token->type == PIPE)
		return (printf("%s: parse error near `%s'\n",
				NAME, token->next->content), false);
	return (true);
}

// static bool	check_first_node(t_treenode *node);

// bool	check_nodes_valid(t_treenode *nodes)
// {
// 	// t_treenode	*temp;
// 	// t_tokentype	type;

// 	// // if (!check_first_node(nodes))
// 	// // 	return (false);
// 	// temp = nodes->left;
// 	// while (temp->left)
// 	// {
// 	// 	type = token_type(temp->cmd);
// 	// 	if (type == ARG || type == WILDCARD)
// 	// 		return (false);
// 	// 	if ((type == PIPE || type == AND || type == OR)
// 	// 		&& temp->args != NULL)
// 	// 		return (false);
// 	// 	temp = temp->left;
// 	// }
// 	return (true);
// }

// static bool	check_first_node(t_treenode *node)
// {

// 	if (token_type(node->cmd) == ARG
// 		|| token_type(node->cmd) == OR
// 		|| token_type(node->cmd) == AND
// 		|| token_type(node->cmd) == PIPE
// 		|| token_type(node->cmd) == REDIR_OUT
// 		|| token_type(node->cmd) == REDIR_IN
// 		|| token_type(node->cmd) == REDIR_APPEND
// 		|| token_type(node->cmd) == HEREDOC)
// 		return (false);
// 	return (true);
// }

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
