/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 18:07:56 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/17 18:43:23 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*process_delimiter(char *delimiter, int *i)
{
	char	*new_del;

	while (delimiter[++i[0]] != '\0')
	{
		if (delimiter[i[0]] == '\'' || delimiter[i[0]] == '"')
			i[1] = 0;
	}
	new_del = remove_quotes(delimiter);
	if (!new_del)
		return (NULL);
	write(0, "> ", 2);
	return (new_del);
}

int	check_delimiter(char **line, char *new_del)
{
	char	*tmp;

	tmp = ft_strtrim(*line, "\n");
	if (ft_strncmp(tmp, new_del, ft_strlen(new_del)) == 0
		&& ft_strlen(tmp) == ft_strlen(new_del))
	{
		free(tmp);
		return (1);
	}
	return (free(tmp), 0);
}

char	*exp_or_cpy(char *line, int flag, int *exp_flags, t_app_data *app)
{
	char	*tmp;

	if (flag == 1)
	{
		tmp = expand(line, app->last_exit_code, app->env_vars, exp_flags);
		if (!tmp)
			return (NULL);
	}
	else
		tmp = ft_strdup(line);
	return (tmp);
}
