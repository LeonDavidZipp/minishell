/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hidden_execve_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:02:08 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/16 18:23:02 by lzipp            ###   ########.fr       */
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
	command = ft_substr(hidden_command, 0, i);
	args = ft_strdup(hidden_command + i);
	if (!command || !args)
		return (free(command), free(args), 1);
	exp_args = expand_and_remove(args, app->last_exit_code,
		app->env_vars, 0);
	if (!exp_args)
		return (free(command), free(args), 1);
	exit_code = execute_cmd(command, exp_args, args, app);
	free(command);
	free(args);
	free(exp_args);
	return (exit_code);
}
