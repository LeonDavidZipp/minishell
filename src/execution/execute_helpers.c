/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 11:56:10 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/04 14:23:06 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// remove path not found error, should just go through the code and print the error at the end of find_path

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
		return (ft_fprintf(2, "%s: Error: PATH not found\n", NAME), NULL); // maybe different error?!
	return (path);
}

char	*find_path(char *command, char **envp)
{
	char	*path;
	char	**temp;
	int		i;

	if (command == NULL || *command == '\0')
		return (ft_fprintf(2, "%s: %s: command not found\n", NAME, command), NULL);
	if (ft_strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (ft_fprintf(2, "%s: %s: No such file or directory\n", NAME, command), NULL);
	}
	temp = ft_split(search_path_variable(envp), ':');
	i = 0;
	while (temp[i] != NULL)
	{
		path = ft_strjoin(temp[i], "/");
		path = ft_join_in_place(path, command);
		if (access(path, X_OK) == 0)
		{
			ft_free_2d_arr((void **)temp);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free_2d_arr((void **)temp);
	return (ft_fprintf(2, "%s: %s: command not found\n", NAME, command), NULL);
}

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

int	is_redir(t_tokentype type)
{
	if (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC)
		return (1);
	return (0);
}
