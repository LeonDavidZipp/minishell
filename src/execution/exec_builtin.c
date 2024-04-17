/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:12:35 by cgerling          #+#    #+#             */
/*   Updated: 2024/04/17 16:25:53 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_error(t_treenode *ast) // also using this in execve
{
	if (ast->err_val == -1)
		ft_fprintf(2,"%s: %s: ambiguous redirect\n", NAME, ast->err);
	else
		ft_fprintf(2,"%s: %s: %s\n", NAME, ast->err, strerror(ast->err_val));
	if (ast->in_fd != 0)
		close(ast->in_fd);
	if (ast->out_fd != 1)
		close(ast->out_fd);
}

int	handle_pipe_builtin(t_builtin var, t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
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

int	handle_fds_dup2(t_treenode *ast) // also using this in execve
{
	if (ast->in_fd != 0)
	{
		if (dup2(ast->in_fd, STDIN_FILENO) == -1)
			return (ft_fprintf(2, "%s: dup2 error: %s\n", NAME, strerror(errno)), 1);
		close(ast->in_fd);
	}
	if (ast->out_fd != 1)
	{
		if (dup2(ast->out_fd, STDOUT_FILENO) == -1)
			return (ft_fprintf(2, "%s: dup2 error: %s\n", NAME, strerror(errno)), 1);
		close(ast->out_fd);
	}
	return (0);
}

t_builtin	init_builtin(t_treenode *ast, t_app_data *app)
{
	t_builtin	var;
	
	var.cmd = NULL;
	var.args = NULL;
	if (ast->err_val != 0)
	{
		handle_error(ast);
		return (var);
	}
	var.cmd = expand_and_remove(ast->cmd, app->last_exit_code, app->env_vars, 0);
	if (ast->args)
		var.args = expand_and_remove(ast->args, app->last_exit_code, app->env_vars, 0);
	else
		var.args = NULL;
	var.exit_code = 0;
	return (var);
}

int	handle_fds_dup(t_builtin *var)
{
	var->stdin_fd = dup(STDIN_FILENO);
	if (var->stdin_fd == -1)
		return (ft_fprintf(2, "%s: dup error: %s\n", NAME, strerror(errno)), 1);
	var->stdout_fd = dup(STDOUT_FILENO);
	if (var->stdout_fd == -1)
		return (ft_fprintf(2, "%s: dup error: %s\n", NAME, strerror(errno)), 1);
	return (0);
}

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
