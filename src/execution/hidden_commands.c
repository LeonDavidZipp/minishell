/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hidden_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:02:08 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/16 18:18:26 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int		is_builtin_no_expand(char *cmd);
int		is_builtin(char *cmd, int exit_code, char **env_vars);
char	*find_path_no_err(char *command, char **envp);
char	*find_path(char *command, char **envp, bool *flag);
int		execute_cmd(char *cmd, char *args, char *ast_args, t_app_data *app);
int		hidden_builtin(char *hidden_command, t_app_data *app);
int		hidden_execve(char *hidden_command, t_app_data *app,
				t_pid_list **pid_list);
int		add_to_pid_list(pid_t pid, t_pid_list **pidlist);
static int	handle_potential_cmd(bool flag, int i, char **command,
				char ***env_vars);
static char	*fill_tmp(char ***args, char **hidden_command);

int	is_hidden_command(char *command, char **env_vars)
{
	int		i;
	bool	flag;

	if (!command)
		return (0);
	else if (ft_strlen(command) == 0)
		return (2);
	i = -1;
	flag = false;
	while (command[++i])
	{
		if (command[i] == ' ')
		{
			flag = true;
			break ;
		}
	}
	if (handle_potential_cmd(flag, i, &command, &env_vars) == 1)
		return (1);
	return (0);
}

static int	handle_potential_cmd(bool flag, int i, char **command,
				char ***env_vars)
{
	char	*potential_path;
	char	*potential_command;

	if (flag)
	{
		potential_command = ft_substr(*command, 0, i);
		if (is_builtin_no_expand(potential_command))
		{
			free(potential_command);
			return (1);
		}
		potential_path = find_path_no_err(potential_command, *env_vars);
		if (potential_path)
		{
			free(potential_command);
			free(potential_path);
			return (1);
		}
	}
	return (0);
}

int	exec_hidden_command(char *hidden_command, char **args,
		t_app_data *app, t_pid_list **pid_list)
{
	char	*check;
	char	*tmp;
	int		i;
	int		exit_code;

	i = 0;
	while (hidden_command[i] && hidden_command[i] != ' ')
		i++;
	check = ft_substr(hidden_command, 0, i);
	if (args[1])
		tmp = fill_tmp(&args, &hidden_command);
	else
		tmp = ft_strdup(hidden_command);
	if (!tmp)
		return (free(check), 1);
	if (is_builtin_no_expand(check))
	{
		exit_code = hidden_builtin(tmp, app);
		return (free(check), free(tmp), exit_code);
	}
	free(check);
	exit_code = hidden_execve(tmp, app, pid_list);
	return (free(tmp), exit_code);
}

static char	*fill_tmp(char ***args, char **hidden_command)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	while ((*args)[i])
	{
		tmp = ft_strjoin(*hidden_command, " ");
		tmp = ft_join_in_place(tmp, (*args)[i]);
		i++;
	}
	return (tmp);
}

int	hidden_builtin(char *hidden_command, t_app_data *app)
{
	char	*command;
	char	*args;
	char	*exp_args;
	int		exit_code;
	int		i;

	i = 0;
	while (hidden_command[i] && hidden_command[i] != ' ')
		i++;
	command = ft_substr(hidden_command, 0, i); // NULL checks
	args = ft_strdup(hidden_command + i);
	exp_args = expand_and_remove(args, app->last_exit_code,
		app->env_vars, 0);
	exit_code = execute_cmd(command, exp_args, args, app);
	free(command);
	free(args);
	free(exp_args);
	return (exit_code);
}

int	hidden_execve(char *hidden_command, t_app_data *app, t_pid_list **pid_list)
{
	pid_t	pid;
	char	*tmp;
	char	**arg_arr;
	int		i;
	int 	flags[2];
	bool	flag = false;

	flags[0] = 0;
	flags[1] = 1;
	tmp = expand(hidden_command, app->last_exit_code,
		app->env_vars, flags); // NULL checks
	arg_arr = split(tmp);
	free(tmp);
	i = 0;
	while (arg_arr[i])
	{
		tmp = expand_and_remove(arg_arr[i], app->last_exit_code,
			app->env_vars, 0);
		if (!tmp)
			return (ft_free_2d_arr((void **)arg_arr), 1);
		free(arg_arr[i]);
		arg_arr[i] = tmp;
		i++;
	}
	pid = fork();
	if (pid == -1)
	{
		ft_fprintf(2, "%s: fork error: %s\n", NAME, strerror(errno));
		return 1;
	}
	if (pid == 0)
	{
		struct stat path_stat;
		stat(arg_arr[0], &path_stat);
		if (S_ISDIR(path_stat.st_mode))
		{
			ft_fprintf(2, "%s: %s: is a directory\n", NAME, arg_arr[0]);
			exit(126);
		}
		if (access(arg_arr[0], X_OK) == 0)
			execve(arg_arr[0], arg_arr, app->env_vars);
		else
			execve(find_path(arg_arr[0], app->env_vars, &flag), arg_arr,
				app->env_vars);
		exit(127);
	}
	else
	{
		if (add_to_pid_list(pid, pid_list))
			return (ft_free_2d_arr((void **)arg_arr), 1);
	}
	ft_free_2d_arr((void **)arg_arr);
	return (0);
}
