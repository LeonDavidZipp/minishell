/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:21:13 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/25 15:46:45 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_token		*new_token(char *content, t_app_data *app);

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
	token->type = token_type(content);
	token->next = NULL;
	free(temp);
	return (token);
}

static char	**join_tokens(char **contents)
{
	char	**new_contents;
	char	*temp;
	int		i;
	int		j;

	i = -1;
	j = -1;
	new_contents = ft_calloc(ft_null_terminated_arr_len((void **)contents) + 1,
			sizeof(char *));
	while (contents[++i])
	{
		if (ft_strcmp(contents[i], "echo") == 0)
		{
			i++;
			if (contents[i] && ft_strcmp(contents[i], "-n") == 0)
				i++;
			while (contents[i] && token_type(contents[i]) == ARG)
			{
				temp = ft_strjoin(new_contents[j], contents[i]);
				free(new_contents[j]);
				new_contents[j] = temp;
				i++;
			}
		}
	}
	return(contents);
}

// int main()
// {
// 	char	*input = ft_strdup("echo hi");
// 	t_token	*tokens;

// 	tokens = tokenize(input);
// 	t_token	*temp = tokens;
// 	int i = 0;
// 	while (temp)
// 	{
// 		printf("content: %s - type: %d\n", temp->content, temp->type);
// 		printf("i: %d\n", i++);
// 		temp = temp->next;
// 	}
// 	printf("done\n");
// 	// free_tokens(tokens);
// 	return (0);
// }
