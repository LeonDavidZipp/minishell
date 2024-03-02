/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:21:13 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/02 14:50:51 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_token	*new_token(char *content, t_token *prev, t_app_data *app);

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
		current = new_token(token_contents[i], prev, app);
		if (!prev)
			first = current;
		else
			prev->next = current;
		prev = current;
	}
	ft_free_2d_arr((void **)token_contents);
	first = join_arg_tokens(first);
	first = join_after_echo(first);
	return (first);
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

static t_token	*new_token(char *content, t_token *prev, t_app_data *app)
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

// int main()
// {
// 	char	*input = ft_strdup("echo -n hi hello u geylord cd && echo hi");
// 	t_token	*tokens;
// 	t_app_data app;
// 	app.input = input;
// 	tokens = tokenize(&app);
// 	t_token	*temp = tokens;
// 	while (temp)
// 	{
// 		printf("content: %s\ntype: %d\n--------\n", temp->content, temp->type);
// 		temp = temp->next;
// 	}
// 	printf("done\n");
// 	free_tokens(tokens);
// 	free(input);
// 	return (0);
// }

// everythin at beginning is cmd
// everything after &&, || is cmd