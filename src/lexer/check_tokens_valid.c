/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens_valid.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 22:01:18 by intra             #+#    #+#             */
/*   Updated: 2024/03/01 06:02:07 by lzipp            ###   ########.fr       */
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
		if (tokens->type == AND || tokens->type == OR)
		{
			if (!tokens->next)
				return (printf("%s: parse error near `\\n'\n", NAME), false);
			else if (tokens->next->type == AND || tokens->next->type == OR
				|| tokens->next->type == PIPE)
				return (printf("%s: parse error near `%s'\n",
						NAME, tokens->next->content), false);
		}
		// > and >>
		if (tokens->type == REDIR_OUT || tokens->type == REDIR_APPEND)
		{
			if (!tokens->next)
				return (printf("%s: parse error near `\\n'\n", NAME), false);
			if (!tokens->next || tokens->next->type == WILDCARD
				|| tokens->next->type == REDIR_IN || tokens->next->type == HEREDOC
				|| tokens->next->type == PIPE
				|| tokens->next->type == AND || tokens->next->type == OR)
				return (printf("%s: parse error near `\\n'\n", NAME), false);
		}
		// < and <<
		if (tokens->type == REDIR_IN || tokens->type == HEREDOC)
		{
			if (!tokens->next || tokens->next->type == WILDCARD
				|| tokens->next->type == REDIR_IN
				|| tokens->next->type == HEREDOC || tokens->next->type == PIPE
				|| tokens->next->type == AND || tokens->next->type == OR)
				return (printf("%s: parse error near `\\n'\n", NAME), false);
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
		|| token->type == PIPE
		|| token->type == WILDCARD)
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
