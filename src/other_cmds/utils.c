/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 11:02:22 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/23 18:26:06 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void		ft_free_tab(char **table);
void			error_exit(char *message, int exit_code);

char	*search_path_variable(char **envp)
{
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	while (envp[i] != NULL)
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (path == NULL)
		error_exit("Error: path variable not found", 1);
	return (path);
}

char	*find_path(char *command, char **envp)
{
	char	*path;
	char	*temp;
	char	**table;
	int		i;

	temp = search_path_variable(envp);
	table = ft_split(temp, ':');
	i = 0;
	while (table[i] != NULL)
	{
		path = ft_strjoin(table[i], "/");
		path = ft_strjoin(path, command);
		if (access(path, X_OK) == 0)
		{
			ft_free_tab(table);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free_tab(table);
	error_exit("Error: command not found\n", 127);
	return (NULL);
}

static void	ft_free_tab(char **table)
{
	int	i;

	i = 0;
	while (table[i])
	{
		free(table[i]);
		i++;
	}
	free(table);
}

void	error_exit(char *message, int exit_code)
{
	ft_putstr_fd(message, 2);
	exit(exit_code);
}
