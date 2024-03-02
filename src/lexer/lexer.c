/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 20:30:46 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 20:30:50 by lzipp            ###   ########.fr       */
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
	// printf("acces to bin: %d\n", access("/bin", F_OK));
	if (check_input(app_data->input) == 1)
	{
		free_for_next_call(app_data);
		return ;
	}
	// 2. then tokenize & 3. expand where needed
	tokens = tokenize(app_data);
	if (!tokens)
		handle_error(app_data);
	t_token *temp = tokens;
	while (temp)
	{
		printf("content: %s\n", temp->content);
		printf("type: %d\n", temp->type);
		temp = temp->next;
	}
	printf("--------------------\n");
	// 4. check if the tree is valid
	if(!check_tokens_valid(tokens))
	{
		free_for_next_call(app_data);
		return ;
	}
	intermediate_tree = combine_cmds_args(tokens);
	free_tokens(tokens);
	if (!intermediate_tree)
		handle_error(app_data);
	// 5. to binary tree (maybe together with step 4)
	ast = build_ast(intermediate_tree);
	free_treenodes(intermediate_tree);
	if (!ast)
		handle_error(app_data);
	debug_printtree(ast, 0);
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

static void	handle_error(t_app_data *app_data)
{
	printf("%s", LEXER_ERR);
	free_for_next_call(app_data);
}
