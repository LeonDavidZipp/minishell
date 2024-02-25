/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:21:13 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/25 20:41:05 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_token		*new_token(char *content, t_app_data *app);
static void			join_tokens(t_token **join, t_token **prev,
						t_token **tokens);
static t_token		*join_arg_tokens(t_token *tokens);

t_token	*tokenize(t_app_data *app)
{
	char			**token_contents;
	t_token			*token;
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
		token = new_token(token_contents[i], app);
		if (!prev)
			first = token;
		else
			prev->next = token;
		prev = token;
	}
	ft_free_2d_arr((void **)token_contents);
	return (join_arg_tokens(first));
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

static t_token	*new_token(char *content, t_app_data *app)
{
	t_token		*token;
	char		*temp;

	if (content[0] == '\'')
		temp = ft_substr(content, 1, ft_strlen(content) - 2);
	else if (content[0] == '\"')
	{
		temp = in_string_expansion(content, app);
		temp = ft_trim_in_place(temp, "\"");
	}
	else if (content[0] == '$')
		temp = in_string_expansion(content, app);
	else
		temp = ft_strdup(content);
	if (!temp)
		return (NULL);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = ft_strdup(temp);
	token->type = token_type(token->content);
	token->next = NULL;
	free(temp);
	return (token);
}

static t_token	*join_arg_tokens(t_token *tokens)
{
	t_token		*first;
	t_token		*prev;
	t_token		*join;

	first = tokens;
	join = NULL;
	prev = NULL;
	while (tokens)
	{
		if (tokens->type == ARG)
			join_tokens(&join, &prev, &tokens);
		else if (tokens->type != ARG)
		{
			join = NULL;
			prev = tokens;
			tokens = tokens->next;
		}
		if (prev)
			prev->next = tokens;
		if (first == tokens && tokens->type == ARG)
			first = join;
	}
	return (first);
}

static void	join_tokens(t_token **join, t_token **prev, t_token **tokens)
{
	if (*join)
	{
		(*join)->content = ft_join_in_place((*join)->content, " ");
		(*join)->content = ft_join_in_place((*join)->content,
				(*tokens)->content);
		(*join)->next = (*tokens)->next;
		free((*tokens)->content);
		free(*tokens);
		*tokens = (*join)->next;
	}
	else
	{
		*join = *tokens;
		*prev = *tokens;
		*tokens = (*tokens)->next;
	}
}

// int main()
// {
// 	char	*input = ft_strdup("echo hi hello u geylord");
// 	t_token	*tokens;
// 	t_app_data app;
// 	app.input = input;
// 	tokens = tokenize(&app);
// 	t_token	*temp = tokens;
// 	int i = 0;
// 	while (temp)
// 	{
// 		printf("content: %s - type: %d\n", temp->content, temp->type);
// 		printf("i: %d\n", i++);
// 		temp = temp->next;
// 	}
// 	printf("done\n");
// 	free_tokens(tokens);
// 	free(input);
// 	return (0);
// }
