/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 21:48:04 by intra             #+#    #+#             */
/*   Updated: 2024/02/27 18:29:12 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_treenode	*new_treenode(t_token *token);

t_treenode	*combine_tokens(t_token *tokens)
{
	t_token			*temp;
	t_treenode		*current;
	t_treenode		*prev;
	t_treenode		*first;

	temp = tokens;
	first = NULL;
	prev = NULL;
	while (temp)
	{
		current = new_treenode(temp);
		if (temp->next && temp->next->type == ARG)
		{
			current->args = ft_strdup(temp->next->content);
			temp = temp->next;
		}
		if (!prev)
			first = current;
		else
			prev->left = current;
		prev = current;
		temp = temp->next;
	}
	free_tokens(tokens);
	return (first);
}

static t_treenode	*new_treenode(t_token *token)
{
	t_treenode	*node;

	node = (t_treenode *)malloc(sizeof(t_treenode));
	if (!node)
		return (NULL);
	node->command = ft_strdup(token->content);
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

int	main(void)
{
	t_app_data	app;
	t_token		*tokens;
	t_treenode	*root;

	app.input = ft_strdup("echo -n hello how are you && cd .. * .");
	tokens = tokenize(&app);
	root = combine_tokens(tokens);
	while (root)
	{
		printf("command: %s\n", root->command);
		printf("args: %s\n-------\n", root->args);
		root = root->left;
	}
	return (0);
}
