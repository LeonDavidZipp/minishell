/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:17:54 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/06 13:23:18 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	here_doc(int argc, char **argv, char **envp)
{
	int		in_out_fd[2];
	int		pipe_fd[2];
	int		exit_status;

	if (argc < 5)
		error_exit("Error: invalid number of arguments\n"
			"Usage: ./pipex here_doc LIMITER cmd ... cmd outfile\n", 1);
	if (pipe(pipe_fd) == -1)
		error_exit("Error: pipe failed\n", 1);
	read_input(argv, pipe_fd, in_out_fd);
	exit_status = execute_commands(argc, argv, envp, in_out_fd);
	close(pipe_fd[0]);
	exit(exit_status);
}

void	read_input(char **argv, int *pipe_fd, int *in_out_fd)
{
	char	*line;
	char	*tmp;

	write(0, "heredoc> ", 9);
	in_out_fd[1] = -1;
	in_out_fd[0] = pipe_fd[0];
	line = get_next_line(0);
	while (line != NULL)
	{
		tmp = ft_strtrim(line, "\n");
		if (ft_strncmp(tmp, argv[2], ft_strlen(argv[2])) == 0
			&& ft_strlen(tmp) == ft_strlen(argv[2]))
			break ;
		free(tmp);
		write(0, "heredoc> ", 9);
		write(pipe_fd[1], line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	get_next_line(-1);
	free(tmp);
	free(line);
	close(pipe_fd[1]);
}

int	execute_commands(int argc, char **argv, char **envp, int *in_out_fd)
{
	int	count;
	int	childs;
	int	status;

	count = 3;
	childs = 0;
	while (count < argc - 2)
	{
		in_out_fd[0] = exec_command(in_out_fd, argv[count], envp);
		childs++;
		count++;
	}
	in_out_fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (in_out_fd[1] == -1)
		error_exit("Error: file failed to open\n", 1);
	in_out_fd[0] = exec_command(in_out_fd, argv[count], envp);
	close(in_out_fd[0]);
	close(in_out_fd[1]);
	while (childs + 1 > 0)
	{
		wait(&status);
		childs--;
	}
	return (WEXITSTATUS(status));
}
