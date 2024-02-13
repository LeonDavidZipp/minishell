/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 11:02:22 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/06 12:30:28 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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
	char	**tab;
	int		i;

	temp = search_path_variable(envp);
	tab = ft_split(temp, ':');
	i = 0;
	while (tab[i] != NULL)
	{
		path = ft_strjoin(tab[i], "/");
		path = ft_strjoin(path, command);
		if (access(path, X_OK) == 0)
		{
			ft_free_tab(tab);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free_tab(tab);
	error_exit("Error: command not found\n", 127);
	return (NULL);
}

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	error_exit(char *message, int exit_code)
{
	ft_putstr_fd(message, 2);
	exit(exit_code);
}

void	check_input(int argc, char **argv, char **envp)
{
	if (BONUS == 1)
	{
		if (argc > 1 && ft_strncmp(argv[1], "here_doc", 8) == 0)
			here_doc(argc, argv, envp);
		if (argc < 4)
			error_exit("Error: invalid number of arguments\n"
				"Usage: ./pipex infile cmd ... cmd outfile\n", 1);
	}
	else
	{
		if (argc != 5)
			error_exit("Error: invalid number of arguments\n"
				"Usage: ./pipex infile cmd1 cmd2 outfile\n", 1);
	}
}
