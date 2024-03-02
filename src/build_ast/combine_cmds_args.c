/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combine_cmds_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 21:48:04 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/02 13:58:08 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_treenode	*new_treenode(char *content, bool in_bracket);
static void			combine_cmds_args_loop(t_token *temp, bool in_bracket,
						t_treenode **first,
						t_treenode **prev);
static void			update_links(t_treenode **first, t_treenode **prev,
						t_treenode *current, t_token **temp);

// t_treenode	*combine_cmds_args(t_token *tokens)
// {
// 	t_token			*temp;
// 	t_treenode		*current;
// 	t_treenode		*prev;
// 	t_treenode		*first;

// 	temp = tokens;
// 	first = NULL;
// 	prev = NULL;
// 	while (temp)
// 	{
// 		current = new_treenode(temp->content);
// 		if (temp->next && temp->next->type == ARG)
// 		{
// 			current->args = ft_strdup(temp->next->content);
// 			temp = temp->next;
// 		}
// 		if (!prev)
// 			first = current;
// 		else
// 			prev->left = current;
// 		prev = current;
// 		temp = temp->next;
// 	}
// 	// free_tokens(tokens);
// 	return (first);
// }

t_treenode	*combine_cmds_args(t_token *tokens)
{
	t_token			*temp;
	// t_treenode		*current;
	t_treenode		*prev;
	t_treenode		*first;
	bool			in_bracket;

	temp = tokens;
	first = NULL;
	prev = NULL;
	in_bracket = false;
	// while (temp)
	// {
	// 	if (temp->type == LEFT_BRACKET)
	// 	{
	// 		temp = temp->next;
	// 		in_bracket = true;
	// 		continue ;
	// 	}
	// 	else if (temp->type == RIGHT_BRACKET)
	// 	{
	// 		temp = temp->next;
	// 		in_bracket = false;
	// 		continue ;
	// 	}
	// 	current = new_treenode(temp->content, in_bracket);
	// 	if (temp->next && temp->next->type == ARG)
	// 	{
	// 		current->args = ft_strdup(temp->next->content);
	// 		temp = temp->next;
	// 	}
	// 	update_links(&first, &prev, current, &temp);
	// }
	combine_cmds_args_loop(temp, in_bracket, &first, &prev);
	return (first);
}

static t_treenode	*new_treenode(char *content, bool in_bracket)
{
	t_treenode	*node;

	node = (t_treenode *)malloc(sizeof(t_treenode));
	if (!node)
		return (NULL);
	node->cmd = ft_strdup(content);
	node->args = NULL;
	node->in_bracket = in_bracket;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static void	combine_cmds_args_loop(t_token *temp, bool in_bracket,
								t_treenode **first, t_treenode **prev)
{
	t_treenode	*current;

	current = NULL;
	while (temp)
	{
		if (temp->type == LEFT_BRACKET)
		{
			temp = temp->next;
			in_bracket = true;
			continue ;
		}
		else if (temp->type == RIGHT_BRACKET)
		{
			temp = temp->next;
			in_bracket = false;
			continue ;
		}
		current = new_treenode(temp->content, in_bracket);
		if (temp->next && temp->next->type == ARG)
		{
			current->args = ft_strdup(temp->next->content);
			temp = temp->next;
		}
		update_links(first, prev, current, &temp);
	}
}

static void	update_links(t_treenode **first, t_treenode **prev,
						t_treenode *current, t_token **temp)
{
	if (!(*prev))
		*first = current;
	else
		(*prev)->left = current;
	*prev = current;
	*temp = (*temp)->next;
}

// int	main(void)
// {
// 	t_app_data	app;
// 	t_token		*tokens;
// 	t_treenode	*root;

// 	app.input = ft_strdup("echo -n hello how are you && cd .. * .");
// 	tokens = tokenize(&app);
// 	root = combine_cmds_args(tokens);
// 	while (root)
// 	{
// 		printf("command: %s\n", root->cmd);
// 		printf("args: %s\n-------\n", root->args);
// 		root = root->left;
// 	}
// 	return (0);
// }
