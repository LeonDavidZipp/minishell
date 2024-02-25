/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intra <intra@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:21:13 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/25 14:32:54 by intra            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_token		*new_token(char *content, t_app_data *app);
static t_tokentype	determine_token_type(char *content);
static t_tokentype	determine_token_type_2(char *content);

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
	else
		temp = ft_strdup(content);
	if (!temp)
		return (NULL);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = ft_strdup(temp);
	token->type = determine_token_type(content);
	token->next = NULL;
	free(temp);
	return (token);
}

static t_tokentype	determine_token_type(char *content)
{
	if (ft_strcmp(content, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(content, "'") == 0)
		return (SINGLE_QUOTE);
	else if (ft_strcmp(content, "\"") == 0)
		return (DOUBLE_QUOTE);
	else if (ft_strcmp(content, "&&") == 0)
		return (AND);
	else if (ft_strcmp(content, "||") == 0)
		return (OR);
	else if (ft_strcmp(content, "*") == 0)
		return (WILDCARD);
	else if (ft_strcmp(content, ">") == 0)
		return (REDIR_OUT);
	else if (ft_strcmp(content, ">>") == 0)
		return (REDIR_APPEND);
	else if (ft_strcmp(content, "<") == 0)
		return (REDIR_IN);
	else if (ft_strcmp(content, "<<") == 0)
		return (HEREDOC);
	else if (content && content[0] == '-')
		return (FLAG);
	else
		return (determine_token_type_2(content));
}

static t_tokentype	determine_token_type_2(char *content)
{
	if (ft_strcmp(content, "echo") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "cd") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "pwd") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "export") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "unset") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "env") == 0)
		return (BUILTIN_CMD);
	else if (ft_strcmp(content, "exit") == 0)
		return (BUILTIN_CMD);
	else
	{
		if (access(content, X_OK) != -1)
			return (OTHER_CMD);
		else
			return (ARG);
	}
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
