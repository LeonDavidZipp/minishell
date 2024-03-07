/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 11:56:10 by cgerling          #+#    #+#             */
/*   Updated: 2024/03/06 19:24:24 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*search_path_variable(char **envp)
{
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (path == NULL)
		return (printf("Error: PATH not found\n"), NULL);
	return (path);
}

char	*find_path(char *command, char **envp)
{
	char	*path;
	char	**temp;
	int		i;

	temp = ft_split(search_path_variable(envp), ':');
	i = 0;
	while (temp[i] != NULL)
	{
		path = ft_strjoin(temp[i], "/");
		path = ft_strjoin(path, command);
		if (access(path, X_OK) == 0)
		{
			ft_free_2d_arr((void **)temp);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free_2d_arr((void **)temp);
	return (printf("Error: Command not found\n"), NULL);
}
