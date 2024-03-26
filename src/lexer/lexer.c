/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:52:11 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/22 17:02:50 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	free_for_next_call(t_app_data *app_data);
static void	handle_error(t_app_data *app_data);

void	lexer(t_app_data *app_data)
{
	t_token		*tokens;
	t_treenode	*intermediate_tree;
	t_treenode	*ast;

	// 1. first validate input (according to braces, quotes, etc.)
	if (check_input(app_data->input) == 1)
	{
		free_for_next_call(app_data);
		return ;
	}
	// 2. then tokenize & 3. expand where needed
	tokens = tokenize(app_data);
	if (!tokens)
		handle_error(app_data);
	// 4. check if the tree is valid
	if(!check_tokens_valid(tokens))
	{
		free_for_next_call(app_data);
		return ;
	}
	intermediate_tree = combine_cmds_args(tokens);
	intermediate_tree = switch_heredocs(intermediate_tree);
	free_tokens(tokens);
	if (!intermediate_tree)
		handle_error(app_data);
	// 5. to binary tree
	ast = NULL;
	ast = build_ast(ast, intermediate_tree, 0);
	free_treenodes(intermediate_tree);
	if (!ast)
		handle_error(app_data);
	// debug_printtree(ast, 0);
	// 6. execute
	execute(app_data, ast);
	g_exit_signal = 0;
	// 7. free memory
	free_treenodes(ast);
	free_for_next_call(app_data);
}

static void	free_for_next_call(t_app_data *app_data)
{
	free(app_data->input);
	app_data->input = NULL;
}

static void	handle_error(t_app_data *app_data)
{
	printf("%s", LEXER_ERR);
	free_for_next_call(app_data);
}
