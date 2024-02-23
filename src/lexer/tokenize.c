/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:21:13 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/23 12:55:17 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_token		*new_token(char *content);
static t_tokentype	determine_token_type(char *content);
static t_tokentype	determine_token_type_2(char *content);

t_token	*true_tokenize(char **input)
{
	t_token			*token;
	t_token			*prev;
	t_token			*first;

	first = NULL;
	prev = NULL;
	while (*input)
	{
		token = new_token(*input);
		if (!prev)
			first = token;
		else
			prev->next = token;
		prev = token;
		input++;
	}
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

static t_token	*new_token(char *content)
{
	t_token		*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = content;
	token->type = determine_token_type(content);
	token->next = NULL;
	return (token);
}

// int main()
// {
// 	char	**input = ft_calloc(4, sizeof(char *));
// 	input[0] = strdup("echo");
// 	input[1] = strdup("*");
// 	input[2] = strdup("world");
// 	t_token	*tokens;

// 	tokens = true_tokenize(input);
// 	t_token	*temp = tokens;
// 	while (temp)
// 	{
// 		printf("content: %s - type: %d\n", temp->content, temp->type);
// 		temp = temp->next;
// 	}
// 	printf("done\n");
// 	free_tokens(tokens);
// 	return (0);
// }
