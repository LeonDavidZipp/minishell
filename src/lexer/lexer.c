/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 11:52:11 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/15 19:03:08 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	free_for_next_call(t_app_data *app_data, t_treenode *ast);
static int	handle_tokens(t_app_data **app_data, int *exit_code,
				t_token **tokens);

int	lexer(t_app_data *app_data)
{
	t_token		*tokens;
	t_treenode	*intermediate_tree;
	t_treenode	*ast;
	int			exit_code;

	exit_code = check_input(app_data->input);
	if (exit_code != 0)
	{
		app_data->last_exit_code = exit_code;
		return (free_for_next_call(app_data, NULL), 1);
	}
	if (handle_tokens(&app_data, &exit_code, &tokens) == 1)
		return (1);
	if (!tokens)
		return (free_for_next_call(app_data, NULL), 1);
	intermediate_tree = switch_heredocs(combine_cmds_args(tokens));
	free_tokens(tokens);
	if (!intermediate_tree)
		return (free_for_next_call(app_data, NULL), 1);
	ast = build_ast(NULL, intermediate_tree, 0);
	free_treenodes(intermediate_tree);
	if (ast)
		execute(app_data, ast);
	g_exit_signal = 0;
	return (free_for_next_call(app_data, ast), 0);
}

static void	free_for_next_call(t_app_data *app_data, t_treenode *ast)
{
	free(app_data->input);
	app_data->input = NULL;
	free_treenodes(ast);
	ast = NULL;
}

int	heredoc_input(char *delimiter)
{
	char	*line;
	char	*tmp;
	char	*new_del;

	g_exit_signal = 2;
	new_del = remove_quotes(delimiter);
	if (!new_del)
		return 1;
	write(0, "> ", 2);
	line = get_next_line(0);
	while (line != NULL)
	{
		tmp = ft_strtrim(line, "\n");
		if (ft_strncmp(tmp, new_del, ft_strlen(new_del)) == 0
			&& ft_strlen(tmp) == ft_strlen(new_del))
			break ;
		free(tmp);
		free(line);
		write(0, "> ", 2);
		line = get_next_line(0);
	}
	if (line)
		g_exit_signal = 0;
	return (free(new_del), free(line), 0);
}

void	check_for_heredoc(t_token *token, int err_loc)
{
	t_token *cur;
	t_token *prev;
	int		i;

	cur = token;
	prev = NULL;
	i = 0;
	// printf("err_loc: %d\n", err_loc);
	while (cur && i <= err_loc)
	{
		if (prev && prev->type == HEREDOC)
		{
			heredoc_input(cur->content);
			g_exit_signal = 0;
		}
		prev = cur;
		cur = cur->next;
		i++;
	}
}

t_token		*new_token(char *content, t_token *prev);

void	prepare_heredoc_check(char *input, int err_loc)
{
	char			**token_contents;
	t_token			*current;
	t_token			*prev;
	t_token			*first;
	int				i;

	token_contents = split(input);
	if (!token_contents)
		return ;
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
	first = remove_after_cd(switch_tokens_for_redir(first));
	check_for_heredoc(first, err_loc);
}

int	handle_tokens(t_app_data **app_data, int *exit_code, t_token **tokens)
{
	int		err_loc;

	err_loc = 0;
	*tokens = tokenize((*app_data)->input, exit_code, &err_loc);
	if (*exit_code != 0)
	{
		prepare_heredoc_check((*app_data)->input, err_loc);
		(*app_data)->last_exit_code = *exit_code;
		free_tokens(*tokens);
		free_for_next_call(*app_data, NULL);
		if ((*app_data)->noninteractive)
			exit(*exit_code);
		return (1);
	}
	return (0);
}
