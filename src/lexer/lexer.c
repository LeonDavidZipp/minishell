/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:52:11 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/08 17:39:02 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	free_for_next_call(t_app_data *app_data, t_treenode *ast);

int	lexer(t_app_data *app_data)
{
	t_token		*tokens;
	t_treenode	*intermediate_tree;
	t_treenode	*ast;
	int			tmp;

	if (check_input(app_data->input) == 1)
		return (free_for_next_call(app_data, NULL), 1);
	tokens = tokenize(app_data->input);
	if (!tokens)
		return (free_for_next_call(app_data, NULL), 1);
	tmp = check_tokens_valid(tokens);
	if (tmp != 0)
	{
		app_data->last_exit_code = tmp;
		free_tokens(tokens);
		return (free_for_next_call(app_data, NULL), 1);
	}
	intermediate_tree = switch_heredocs(combine_cmds_args(tokens));
	free_tokens(tokens);
	if (!intermediate_tree)
		return (free_for_next_call(app_data, NULL), 1);
	ast = build_ast(NULL, intermediate_tree, 0);
	free_treenodes(intermediate_tree);
	debug_printtree(ast, 0);
	if (ast)
		execute(app_data, ast);
	g_exit_signal = 0;
	return (free_for_next_call(app_data, ast), 0);
}

static void	free_for_next_call(t_app_data *app_data, t_treenode *ast)
{
	free(app_data->input);
	app_data->input = NULL;
	free_treenodes(ast);
	ast = NULL;
}
