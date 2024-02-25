/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:52:11 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/25 14:30:29 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	free_for_next_call(t_app_data *app_data);

void	lexer(t_app_data *app_data)
{
	t_token		*tokens;
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
	while (tokens)
	{
		printf("content: %s, type: %d\n", tokens->content, tokens->type);
		tokens = tokens->next;
	}
	// t_token	*tmp = tokens;
	// printf("tokens:\n");
	// while (tmp)
	// {
	// 	printf("content: %s, type: %d\n", tmp->content, tmp->type);
	// 	tmp = tmp->next;
	// }

	// 4. write to linked list including type of token
	// 5. to binary tree (maybe together with step 4)
	// 6. execute
	// 7. free memory
}

static void	free_for_next_call(t_app_data *app_data)
{
	(void)app_data;
	// free(app_data->input);
	// app_data->input = NULL;
}
