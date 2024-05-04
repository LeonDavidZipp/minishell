/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:06:53 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/16 16:09:43 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	check_for_heredoc(t_token *token, int err_loc);
static int	heredoc_input(char *delimiter);

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

static void	check_for_heredoc(t_token *token, int err_loc)
{
	t_token		*cur;
	t_token		*prev;
	int			i;

	cur = token;
	prev = NULL;
	i = 0;
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

static int	heredoc_input(char *delimiter)
{
	char	*line;
	char	*tmp;
	char	*new_del;

	g_exit_signal = 2;
	new_del = remove_quotes(delimiter);
	if (!new_del)
		return (1);
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
