/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 14:21:13 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/23 11:55:52 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	update_links(t_token **first, t_token **prev, t_token *env_var);

t_tokentype	determine_token_type(char *content)
{
	// obvious rulings: everything that can be directly assigned to a token type
	// ############################################################
	printf("content: %s\n", content);
	printf("strncmp: %d\n", ft_strncmp(content, "echo", ft_strlen("echo")));
	if (ft_strncmp(content, "|", ft_strlen("|") == 0))
		return (PIPE);
	else if (ft_strncmp(content, "'", ft_strlen("'") == 0))
		return (SINGLE_QUOTE);
	else if (ft_strncmp(content, "\"", ft_strlen("\"") == 0))
		return (DOUBLE_QUOTE);
	else if (ft_strncmp(content, "&&", ft_strlen("&&") == 0))
		return (AND);
	else if (ft_strncmp(content, "||", ft_strlen("||") == 0))
		return (OR);
	else if (ft_strncmp(content, "*", ft_strlen("*") == 0))
		return (WILDCARD);
	else if (ft_strncmp(content, ">", ft_strlen(">") == 0))
		return (REDIR_OUT);
	else if (ft_strncmp(content, ">>", ft_strlen(">>") == 0))
		return (REDIR_APPEND);
	else if (ft_strncmp(content, "<", ft_strlen("<") == 0))
		return (REDIR_IN);
	else if (ft_strncmp(content, "<<", ft_strlen("<<") == 0))
		return (HEREDOC);
	else if (ft_strncmp(content, "echo", ft_strlen("echo") == 0))
		return (BUILTIN_CMD);
	else if (ft_strncmp(content, "cd", ft_strlen("cd") == 0))
		return (BUILTIN_CMD);
	else if (ft_strncmp(content, "pwd", ft_strlen("pwd") == 0))
		return (BUILTIN_CMD);
	else if (ft_strncmp(content, "export", ft_strlen("export") == 0))
		return (BUILTIN_CMD);
	else if (ft_strncmp(content, "unset", ft_strlen("unset") == 0))
		return (BUILTIN_CMD);
	else if (ft_strncmp(content, "env", ft_strlen("env") == 0))
		return (BUILTIN_CMD);
	else if (ft_strncmp(content, "exit", ft_strlen("exit") == 0))
		return (BUILTIN_CMD);
	else if (content && content[0] == '-')
		return (FLAG);
	// ############################################################
	// everything below here is either an execve command or an argument
	// if its an execve command, access will return 0
	else
	// else if (access(content, F_OK) != -1)
	{
		// file exists
		if (access(content, X_OK) != -1)
		{
			// file is executable
			return (OTHER_CMD);
		}
		else
		{
			// file is not executable
			return (ARG);
		}
	}
}

t_token	*new_token(char *content)
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
		update_links(&first, &prev, token);
		input++;
	}
	return (first);
}

static void	update_links(t_token **first, t_token **prev, t_token *env_var)
{
	if (!*prev)
		*first = env_var;
	else
		(*prev)->next = env_var;
	*prev = env_var;
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

int main()
{
	char	**input = ft_calloc(4, sizeof(char *));
	input[0] = "echo";
	input[1] = "*";
	input[2] = "world";
	t_token	*tokens;

	tokens = true_tokenize(input);
	t_token	*temp = tokens;
	while (temp)
	{
		printf("content: %s - type: %d\n", temp->content, temp->type);
		temp = temp->next;
	}
	printf("done\n");
	free_tokens(tokens);
	return (0);
}
