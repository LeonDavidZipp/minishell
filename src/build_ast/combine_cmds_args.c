/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combine_cmds_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 21:48:04 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/20 15:26:33 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void			combine_cmds_args_loop(t_token *temp, int bracket_lvl,
						t_treenode **first, t_treenode **prev);
static void			update_links(t_treenode **first, t_treenode **prev,
						t_treenode *current, t_token **temp);

t_treenode	*combine_cmds_args(t_token *tokens)
{
	t_token			*temp;
	t_treenode		*prev;
	t_treenode		*first;
	int				bracket_lvl;

	temp = tokens;
	first = NULL;
	prev = NULL;
	bracket_lvl = 0;
	combine_cmds_args_loop(temp, bracket_lvl, &first, &prev);
	return (first);
}

static void	combine_cmds_args_loop(t_token *temp, int bracket_lvl,
								t_treenode **first, t_treenode **prev)
{
	t_treenode	*current;

	current = NULL;
	while (temp)
	{
		if (temp->type == LEFT_BRACKET)
		{
			temp = temp->next;
			bracket_lvl += 1;
			continue ;
		}
		else if (temp->type == RIGHT_BRACKET)
		{
			temp = temp->next;
			bracket_lvl -= 1;
			continue ;
		}
		current = new_treenode(temp->content, NULL, temp->type, bracket_lvl);
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
// 	t_treenode	*temp = root;
// 	while (root)
// 	{
// 		printf("command: %s\n", root->cmd);
// 		printf("args: %s\n-------\n", root->args);
// 		root = root->left;
// 	}
// 	free_tokens(tokens);
// 	free_treenodes(temp);
// 	free(app.input);
// 	return (0);
// }
