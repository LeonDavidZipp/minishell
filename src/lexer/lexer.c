/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:52:11 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/27 15:43:32 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	free_for_next_call(t_app_data *app_data, t_treenode *ast);

void	lexer(t_app_data *app_data)
{
	t_token		*tokens;
	t_treenode	*intermediate_tree;
	t_treenode	*ast;

	if (check_input(app_data->input) == 1)
	{
		free_for_next_call(app_data, NULL);
		return ;
	}
	tokens = tokenize(app_data->input);
	if (!tokens || !check_tokens_valid(tokens))
	{
		app_data->last_exit_code = 2;
		free_for_next_call(app_data, NULL);
		return ;
	}
	intermediate_tree = switch_heredocs(combine_cmds_args(tokens));
	free_tokens(tokens);
	if (!intermediate_tree)
		free_for_next_call(app_data, NULL);
	ast = build_ast(NULL, intermediate_tree, 0);
	free_treenodes(intermediate_tree);
	if (ast)
		execute(app_data, ast);
	g_exit_signal = 0;
	free_for_next_call(app_data, ast);
}

static void	free_for_next_call(t_app_data *app_data, t_treenode *ast)
{
	free(app_data->input);
	app_data->input = NULL;
	free_treenodes(ast);
	ast = NULL;
}
