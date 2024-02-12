/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 13:03:23 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/06 13:22:53 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <sys/types.h>

# ifndef BONUS
#  define BONUS 0
# endif

typedef struct s_split
{
	int		flag_1;
	int		flag_2;
	int		count;
	int		i;
	int		j;
	int		str_index;
	char	**str;
}			t_split;

void	error_exit(char *message, int exit_code);
char	*find_path(char *command, char **envp);
char	*search_path_variable(char **envp);
void	ft_free_tab(char **s);
void	handle_processes(int argc, char **argv, char **envp, int *in_out_fd);
int		exec_command(int *in_out_fd, char *command, char **envp);
void	exec_child(int *in_out_fd, int *pipe_fd, char *command, char **envp);
void	here_doc(int argc, char **argv, char **envp);
void	read_input(char **argv, int *pipe_fd, int *in_out_fd);
int		execute_commands(int argc, char **argv, char **envp, int *in_out_fd);
void	init_struct(t_split *var);
int		count_word(char const *s, char c);
char	**split(char const *s, char c);
char	*process_substring(t_split *var, char const *s, char c);
void	check_input(int argc, char **argv, char **envp);

#endif // PIPEX_H