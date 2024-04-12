/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 11:56:10 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/12 17:07:29 by cgerling         ###   ########.fr       */
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
		return (NULL);
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
		return (ft_fprintf(2, "%s: %s: %s\n", NAME, command, strerror(errno)), NULL);
	}
	temp = ft_split(search_path_variable(envp), ':');
	if (!temp)
		return (ft_fprintf(2, "%s: %s: No such file or directory\n", NAME, command), NULL);
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
	char	*tmp;

	tmp = remove_quotes(cmd);
	if (ft_strcmp(tmp, "cd") == 0 || ft_strcmp(tmp, "pwd") == 0
		|| ft_strcmp(tmp, "echo") == 0 || ft_strcmp(tmp, "env") == 0
		|| ft_strcmp(tmp, "exit") == 0 || ft_strcmp(tmp, "export") == 0
		|| ft_strcmp(tmp, "unset") == 0)
		return (free(tmp), 1);
	free(tmp);
	return (0);
}

int	is_redir(t_tokentype type)
{
	if (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC)
		return (1);
	return (0);
}
