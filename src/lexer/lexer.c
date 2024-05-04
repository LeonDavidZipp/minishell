/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:52:11 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/16 16:07:26 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	free_for_next_call(t_app_data *app_data, t_treenode *ast);
static int	handle_tokens(t_app_data **app_data, int *exit_code,
				t_token **tokens);

int	lexer(t_app_data *app_data)
{
	t_token		*tokens;
	t_treenode	*intermediate_tree;
	t_treenode	*ast;
	int			exit_code;

	exit_code = check_input(app_data->input);
	if (exit_code != 0)
	{
		app_data->last_exit_code = exit_code;
		return (free_for_next_call(app_data, NULL), 1);
	}
	if (handle_tokens(&app_data, &exit_code, &tokens) == 1)
		return (1);
	if (!tokens)
		return (free_for_next_call(app_data, NULL), 1);
	intermediate_tree = switch_heredocs(combine_cmds_args(tokens));
	free_tokens(tokens);
	if (!intermediate_tree)
		return (free_for_next_call(app_data, NULL), 1);
	ast = build_ast(NULL, intermediate_tree, 0);
	free_treenodes(intermediate_tree);
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

int	handle_tokens(t_app_data **app_data, int *exit_code, t_token **tokens)
{
	int		err_loc;

	err_loc = 0;
	*tokens = tokenize((*app_data)->input, exit_code, &err_loc);
	if (*exit_code != 0)
	{
		prepare_heredoc_check((*app_data)->input, err_loc);
		(*app_data)->last_exit_code = *exit_code;
		free_tokens(*tokens);
		free_for_next_call(*app_data, NULL);
		if ((*app_data)->noninteractive)
			exit(*exit_code);
		return (1);
	}
	return (0);
}
