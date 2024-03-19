/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:21:13 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/19 10:33:35 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_token		*new_token(char *content, t_token *prev);

t_token	*tokenize(t_app_data *app)
{
	char			**token_contents;
	t_token			*current;
	t_token			*prev;
	t_token			*first;
	int				i;

	token_contents = split(app->input, app->last_exit_code);
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
	t_token	*temp = first;
	printf("before any switching\n");
	while (temp)
	{
		printf("content: %s, type: %d\n", temp->content, temp->type);
		temp = temp->next;
	}
	printf("--------------------\n");
	ft_free_2d_arr((void **)token_contents);
	printf("before redir switching\n");
	while (temp)
	{
		printf("content: %s, type: %d\n", temp->content, temp->type);
		temp = temp->next;
	}
	printf("--------------------\n");
	first = switch_tokens_for_redir(first);
	printf("before joining args\n");
	while (temp)
	{
		printf("content: %s, type: %d\n", temp->content, temp->type);
		temp = temp->next;
	}
	printf("--------------------\n");
	first = join_arg_tokens(first);
	printf("before after echo join\n");
	while (temp)
	{
		printf("content: %s, type: %d\n", temp->content, temp->type);
		temp = temp->next;
	}
	printf("--------------------\n");
	return (join_after_echo(first));
}

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
