/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:52:11 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/23 17:07:05 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	lexer(t_app_data *app_data)
{
	char	**tokens;
	// 1. first validate input (according to braces, quotes, etc.)
	if (check_input(app_data->input) == 1)
	{
		free_app_data(app_data);
		// exit(1);
	}
	// 2. then tokenize
	tokens = tokenize(app_data->input);
	if (!tokens)
	{
		printf("%s", LEXER_ERR);
		free_app_data(app_data);
		// exit(1);
	}
	// 3. expand where needed

	// 4. write to linked list including type of token
	// 5. to binary tree (maybe together with step 4)
	// 6. execute
	// 7. free memory
}
