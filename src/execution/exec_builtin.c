/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:12:35 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/17 18:35:54 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_builtin	init_builtin(t_treenode *ast, t_app_data *app);
static int			handle_fds_dup(t_builtin *var);
static int			handle_pipe_builtin(t_builtin var, t_treenode *ast,
						t_app_data *app, t_pid_list **pid_list);

int	execute_builtin(t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
{
	t_builtin	var;

	var = init_builtin(ast, app);
	if (!var.cmd || (ast->args && !var.args))
		return (1);
	if (handle_fds_dup(&var) || handle_fds_dup2(ast))
		return (1);
	if (ast->pipe == true)
	{
		if (handle_pipe_builtin(var, ast, app, pid_list))
			return (1);
	}
	else
		var.exit_code = execute_cmd(var.cmd, var.args, ast->args, app);
	if (dup2(var.stdin_fd, STDIN_FILENO) == -1)
		return (ft_fprintf(2, "%s: dup2: %s\n", NAME, strerror(errno)), 1);
	close(var.stdin_fd);
	if (dup2(var.stdout_fd, STDOUT_FILENO) == -1)
		return (ft_fprintf(2, "%s: dup2: %s\n", NAME, strerror(errno)), 1);
	close(var.stdout_fd);
	free(var.cmd);
	if (ast->args)
		free(var.args);
	return (var.exit_code);
}

static t_builtin	init_builtin(t_treenode *ast, t_app_data *app)
{
	t_builtin	var;

	var.cmd = NULL;
	var.args = NULL;
	if (ast->err_val != 0)
	{
		handle_error(ast);
		return (var);
	}
	var.cmd = expand_and_remove(ast->cmd, app->last_exit_code,
			app->env_vars, 0);
	if (ast->args)
		var.args = expand_and_remove(ast->args, app->last_exit_code,
				app->env_vars, 0);
	else
		var.args = NULL;
	var.exit_code = 0;
	return (var);
}

static int	handle_pipe_builtin(t_builtin var, t_treenode *ast,
				t_app_data *app, t_pid_list **pid_list)
{
	var.pid = fork();
	if (var.pid == -1)
	{
		ft_fprintf(2, "%s: fork error: %s\n", NAME, strerror(errno));
		return (1);
	}
	if (var.pid == 0)
	{
		var.exit_code = execute_cmd(var.cmd, var.args, ast->args, app);
		close_fds_loop();
		exit(var.exit_code);
	}
	else
	{
		if (add_to_pid_list(var.pid, pid_list))
			return (1);
	}
	return (0);
}

int	execute_cmd(char *cmd, char *args, char *ast_args, t_app_data *app)
{
	int	exit_code;

	exit_code = 0;
	if (ft_strcmp(cmd, "cd") == 0)
		exit_code = builtin_cd(args, &app->env_vars, &app->last_exit_code);
	else if (ft_strcmp(cmd, "pwd") == 0)
		exit_code = builtin_pwd(args, STDOUT_FILENO);
	else if (ft_strcmp(cmd, "echo") == 0)
		exit_code = builtin_echo(ast_args, STDOUT_FILENO, &app);
	else if (ft_strcmp(cmd, "env") == 0)
		exit_code = builtin_env(args, &app->env_vars);
	else if (ft_strcmp(cmd, "exit") == 0)
		exit_code = builtin_exit(app, args);
	else if (ft_strcmp(cmd, "export") == 0)
		exit_code = builtin_export(ast_args, &app, STDOUT_FILENO);
	else if (ft_strcmp(cmd, "unset") == 0)
		exit_code = builtin_unset(args, app->env_vars);
	else
		exit_code = 127;
	return (exit_code);
}

static int	handle_fds_dup(t_builtin *var)
{
	var->stdin_fd = dup(STDIN_FILENO);
	if (var->stdin_fd == -1)
		return (ft_fprintf(2, "%s: dup error: %s\n", NAME, strerror(errno)), 1);
	var->stdout_fd = dup(STDOUT_FILENO);
	if (var->stdout_fd == -1)
		return (ft_fprintf(2, "%s: dup error: %s\n", NAME, strerror(errno)), 1);
	return (0);
}
