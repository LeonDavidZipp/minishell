/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:52:11 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/28 18:58:47 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	free_for_next_call(t_app_data *app_data);

void	lexer(t_app_data *app_data)
{
	t_token		*tokens;
	t_treenode	*intermediate_tree;
	t_treenode	*ast;
	// 1. first validate input (according to braces, quotes, etc.)
	if (check_input(app_data->input) == 1)
		free_for_next_call(app_data);
	// 2. then tokenize & ...
	// 3. expand where needed
	tokens = tokenize(app_data);
	if (!tokens)
	{
		printf("%s", LEXER_ERR);
		free_for_next_call(app_data);
	}
	// 4. write to linked list including type of token
	intermediate_tree = combine_cmds_args(tokens);
	free_tokens(tokens);
	if (!intermediate_tree)
	{
		printf("%s", LEXER_ERR);
		free_for_next_call(app_data);
	}
	// 4. check if the tree is valid
	// if (!check_nodes_valid(intermediate_tree))
	// {
	// 	printf("%s", LEXER_ERR);
	// 	free_for_next_call(app_data);
	// }
	// 5. to binary tree (maybe together with step 4)
	ast = build_ast(intermediate_tree);
	free_treenodes(intermediate_tree);
	if (!ast)
	{
		printf("%s", LEXER_ERR);
		free_for_next_call(app_data);
	}
	debug_printtree(ast, 0);
	// t_token	*tmp = tokens;
	// while (tmp)
	// {
	// 	printf("content: %s\ntype: %d\n-------\n", tmp->content, tmp->type);
	// 	tmp = tmp->next;
	// }
	printf("lexer done\n");
	// 6. execute
	// execute(ast);
	// 7. free memory
	free_treenodes(ast);
	free_for_next_call(app_data);
}

static void	free_for_next_call(t_app_data *app_data)
{
	free(app_data->input);
	app_data->input = NULL;
}
