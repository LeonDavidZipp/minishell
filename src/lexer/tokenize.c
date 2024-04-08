/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:21:13 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/08 17:31:31 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_token		*new_token(char *content, t_token *prev);

t_token	*tokenize(char *input)
{
	char			**token_contents;
	t_token			*current;
	t_token			*prev;
	t_token			*first;
	int				i;

	token_contents = split(input);
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
	first = remove_after_cd(first);
	first = switch_tokens_for_redir(first);
	return (join_after_ls(join_after_echo(join_arg_tokens(first))));
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
