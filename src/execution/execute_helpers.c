/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 11:56:10 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/17 15:26:46 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// remove path not found error, should just go through
// the code and print the error at the end of find_path

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

char	*find_path(char *command, char **envp, bool *flag)
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
		if (errno == EACCES)
			*flag = true;
		return (ft_fprintf(2, "%s: %s: %s\n", NAME, command, strerror(errno)), NULL);
	}
	temp = ft_split(search_path_variable(envp), ':');
	if (!temp)
	{
		struct stat path_stat;
		stat(command, &path_stat); // check for stat failure
		if (S_ISREG(path_stat.st_mode))
		{
			if (access(command, X_OK) == 0)
				return (ft_strdup(command));
			if (errno == EACCES)
				*flag = true;
			return (ft_fprintf(2, "%s: %s: %s\n", NAME, command, strerror(errno)), NULL);
		}
		ft_fprintf(2, "%s: %s: No such file or directory\n", NAME, command);
		exit(1);
	}
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

char	*find_path_no_err(char *command, char **envp)
{
	char	*path;
	char	**temp;
	int		i;

	if (command == NULL || *command == '\0')
		return (NULL);
	if (ft_strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	temp = ft_split(search_path_variable(envp), ':');
	if (!temp)
		return (NULL);
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
	return (NULL);
}

int	is_builtin(char *cmd, int exit_code, char **env_vars)
{
	char	*tmp;

	tmp = expand_and_remove(cmd, exit_code, env_vars, 0);
	if (ft_strcmp(tmp, "cd") == 0 || ft_strcmp(tmp, "pwd") == 0
		|| ft_strcmp(tmp, "echo") == 0 || ft_strcmp(tmp, "env") == 0
		|| ft_strcmp(tmp, "exit") == 0 || ft_strcmp(tmp, "export") == 0
		|| ft_strcmp(tmp, "unset") == 0)
		return (free(tmp), 1);
	free(tmp);
	return (0);
}

int	is_builtin_no_expand(char *cmd)
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
