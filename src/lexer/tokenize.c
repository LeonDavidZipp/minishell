/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:21:13 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/08 12:23:37 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_token	*new_token(char *content, t_token *prev);
t_token 		*switch_args_for_redir(t_token *token);

t_token	*tokenize(t_app_data *app)
{
	char			**token_contents;
	t_token			*current;
	t_token			*prev;
	t_token			*first;
	int				i;

	token_contents = split(app->input);
	if (!token_contents)
		return (NULL);
	first = NULL;
	prev = NULL;
	i = -1;
	while (token_contents[++i])
	{
		current = new_token(token_contents[i], prev);
		if (!prev)
			first = current;
		else
			prev->next = current;
		prev = current;
	}
	ft_free_2d_arr((void **)token_contents);
	first = switch_args_for_redir(first);
	first = join_arg_tokens(first);
	first = join_after_echo(first);
	return (first);
}
// reorder tokens for redirections
// >: arguments can be like this: arg1 > file1 arg2 ... argn
// >>: exactly as above
// after any redirect the first character or word is the argument and 
// anything after that is an argument for the command

t_token *switch_args_for_redir(t_token *token)
{
	t_token *current;
	t_token *temp;

	current = token;
	while (current && current->next && current->next->next)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_IN
			|| current->type REDIR_APPEND || current->type == HEREDOC)
		{
			if (current->next->next->type == ARG)
			{
				temp = current->next->next;
				current->next->next = temp->next;
				temp->next = current->next;
				current->next = temp;
			}
		}
		current = current->next;
	}
	return (token);
}


// {
// 	t_token *temp;
// 	int redir_index;
// 	int args_start_index;

// 	redir_index = -1;
// 	while (arr && arr[++redir_index])
// 	{
// 		args_start_index = 0;
// 		if (arr[redir_index]->type == TOK_REDIR)
// 			args_start_index = redir_index + 1;
// 		if (args_start_index > 0 && arr[++args_start_index] && arr[args_start_index]->type == TOK_CMD_ARG)
// 		{
// 			temp = arr[args_start_index];
// 			arr[args_start_index] = arr[redir_index + 1];
// 			arr[redir_index + 1] = arr[redir_index];
// 			arr[redir_index] = temp;
// 		}
// 	}
// 	return (arr);
// }

void	free_tokens(t_token *token)
{
	t_token	*temp;

	while (token)
	{
		temp = token->next;
		free(token->content);
		free(token);
		token = temp;
	}
}

static t_token	*new_token(char *content, t_token *prev)
{
	t_token		*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = ft_strdup(content);
	if (prev)
		token->type = token_type(token->content, prev->type);
	else
		token->type = token_type(token->content, FIRST);
	token->next = NULL;
	return (token);
}
