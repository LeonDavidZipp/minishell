/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:21:13 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/29 16:26:12 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_token		*new_token(char *content, t_app_data *app);

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
		current = new_token(token_contents[i], app);
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

static t_token	*new_token(char *content, t_app_data *app)
{
	t_token		*token;
	char		*temp;
	char		*path;

	// cut out as soon as charlotte is done
	if (content[0] == '\'')
		temp = ft_substr(content, 1, ft_strlen(content) - 2);
	else if (content[0] == '\"' || content[0] == '$')
	{
		temp = in_string_expansion(content, app);
		temp = ft_trim_in_place(temp, "\"");
	}
	else
		temp = ft_strdup(content);
	if (!temp)
		return (NULL);
	// end cut out
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	path = get_path(app->env_vars);
	token->content = ft_strdup(temp);
	token->type = token_type(token->content, path);
	token->next = NULL;
	free(temp);
	free(path);
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
