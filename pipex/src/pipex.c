/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 13:15:44 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/05 18:55:15 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int	in_out_fd[2];

	check_input(argc, argv, envp);
	in_out_fd[0] = open(argv[1], O_RDONLY);
	if (in_out_fd[0] == -1)
		error_exit("Error: file failed to open\n", 1);
	in_out_fd[1] = -1;
	handle_processes(argc, argv, envp, in_out_fd);
	return (0);
}

void	handle_processes(int argc, char **argv, char **envp, int *in_out_fd)
{
	int	count;
	int	childs;
	int	status;

	count = 2;
	childs = 0;
	while (count < argc - 2)
	{
		in_out_fd[0] = exec_command(in_out_fd, argv[count], envp);
		childs++;
		count++;
	}
	in_out_fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
	exit(WEXITSTATUS(status));
}

int	exec_command(int *in_out_fd, char *command, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		error_exit("Error: pipe failed\n", 1);
	pid = fork();
	if (pid == -1)
		error_exit("Error: fork failed\n", 1);
	if (pid == 0)
		exec_child(in_out_fd, pipe_fd, command, envp);
	else
		close(pipe_fd[1]);
	close(in_out_fd[0]);
	if (in_out_fd[1] == -1)
		return (pipe_fd[0]);
	close(pipe_fd[0]);
	return (in_out_fd[1]);
}

void	exec_child(int *in_out_fd, int *pipe_fd, char *command, char **envp)
{
	char	**cmd;

	close(pipe_fd[0]);
	dup2(in_out_fd[0], STDIN_FILENO);
	if (in_out_fd[1] != -1)
		dup2(in_out_fd[1], STDOUT_FILENO);
	else
		dup2(pipe_fd[1], STDOUT_FILENO);
	close(in_out_fd[0]);
	if (in_out_fd[1] != -1)
		close(in_out_fd[1]);
	close(pipe_fd[1]);
	cmd = split(command, ' ');
	if (!cmd)
		error_exit("Error: split failed\n", 1);
	if (execve(find_path(cmd[0], envp), cmd, envp) == -1)
		error_exit("Error: command failed to execute\n", 1);
}
