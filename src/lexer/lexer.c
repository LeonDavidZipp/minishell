/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:52:11 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/29 14:34:18 by lzipp            ###   ########.fr       */
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
	printf("token type ls: %d\n", token_type("ls"));
	printf("access code: %d\n", access("/bin/ls", X_OK));
	if (check_input(app_data->input) == 1)
	{
		// print some shit
		free_for_next_call(app_data);
	}
	// 2. then tokenize & 3. expand where needed
	tokens = tokenize(app_data);
	t_token *temp = tokens;
	while (temp)
	{
		printf("content: %s\n", temp->content);
		temp = temp->next;
	}
	printf("\n-------\n");
	if (!tokens)
		handle_error(app_data);
	// 4. check if the tree is valid
	// if (!check_nodes_valid(tokens))
	// {
	// 	printf("%s", LEXER_ERR);
	// 	free_for_next_call(app_data);
	// }
	intermediate_tree = combine_cmds_args(tokens);
	free_tokens(tokens);
	if (!intermediate_tree)
		handle_error(app_data);
	t_treenode *temp2 = intermediate_tree;
	while (temp2)
	{
		printf("cmds & args: %s | %s\n", temp2->cmd, temp2->args);
		temp2 = temp2->left;
	}
	// 5. to binary tree (maybe together with step 4)
	printf("building ast\n");
	ast = build_ast(intermediate_tree);
	printf("lexer done wit ast\n");
	free_treenodes(intermediate_tree);
	printf("lexer done with free\n");
	if (!ast)
		handle_error(app_data);
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

static void	handle_error(t_app_data *app_data)
{
	printf("%s", LEXER_ERR);
	free_for_next_call(app_data);
}
