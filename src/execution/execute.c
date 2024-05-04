/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/03/28 16:06:18 by lzipp            ###   ########.fr       */
=======
/*   Updated: 2024/04/17 18:51:13 by lzipp            ###   ########.fr       */
>>>>>>> ff9c7fa00235e7e7620e61b21306033764e37de9
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_for_errors(t_treenode *ast, int last_exit_code);

int	execute(t_app_data *app, t_treenode *ast)
{
	t_pid_list	*pid_list;
	int			ret;

	ret = 0;
	pid_list = NULL;
	ret = 0;
	setup_fd(ast, app, &ret);
	if (ret == 2 || g_exit_signal == 2)
	{
		app->last_exit_code = 1;
		return (1);
	}
	exec_cmds(ast, app, &pid_list);
	wait_and_free(app, &pid_list);
	return (0);
}

void	exec_cmds(t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
{
	if (ast == NULL)
		return ;
	if (ast->left)
		exec_cmds(ast->left, app, pid_list);
	if (ast->cmd_type == CMD)
	{
		if (is_builtin(ast->cmd, app->last_exit_code, app->env_vars))
			app->last_exit_code = execute_builtin(ast, app, pid_list);
		else
			app->last_exit_code = execute_execve(ast, app, pid_list);
	}
	else if (ast->cmd_type == PIPE || is_redir(ast->cmd_type))
		app->last_exit_code = check_for_errors(ast, app->last_exit_code);
	else if (ast->cmd_type == AND || ast->cmd_type == OR)
	{
		wait_and_free(app, pid_list);
		if ((ast->cmd_type == AND && app->last_exit_code == 0)
			|| (ast->cmd_type == OR && app->last_exit_code != 0))
			exec_cmds(ast->right, app, pid_list);
	}
	if (ast->cmd_type != AND && ast->cmd_type != OR)
	{
		if (ast->right)
			exec_cmds(ast->right, app, pid_list);
	}
}

int	check_for_errors(t_treenode *ast, int last_exit_code)
{
	if (ast->err_val != 0)
	{
		ft_fprintf(2, "%s: %s: %s\n", NAME, ast->err, strerror(ast->err_val));
		return (1);
	}
	if (last_exit_code != 0)
		return (last_exit_code);
	return (0);
}

void	wait_and_free(t_app_data *app, t_pid_list **pid_list)
{
	t_pid_list	*tmp;
	t_pid_list	*next;
	int			status;
	int			exit_code;

	g_exit_signal = 1;
	tmp = *pid_list;
	exit_code = 0;
	while (tmp)
	{
		waitpid(tmp->pid, &status, 0);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			exit_code = 128 + WTERMSIG(status);
		}
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	if (app->last_exit_code == 0)
		app->last_exit_code = exit_code;
	*pid_list = NULL;
	g_exit_signal = 0;
}

int	add_to_pid_list(pid_t pid, t_pid_list **pidlist)
{
	t_pid_list	*new;
	t_pid_list	*tmp;

	new = (t_pid_list *)malloc(sizeof(t_pid_list));
	if (!new)
		return (1);
	new->pid = pid;
	new->next = NULL;
	if (*pidlist == NULL)
		*pidlist = new;
	else
	{
		tmp = *pidlist;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (0);
}
<<<<<<< HEAD

static int	execute_execve(t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
{
	pid_t	pid;
	char	**arg_arr;
	char	*cmd_node;
	char	*tmp;
	int		fd;

	cmd_node = ft_strjoin(ast->cmd, " ");
	if (!cmd_node)
		return (1);
	if (ast->args)
	{
		tmp = ft_strjoin(cmd_node, ast->args);
		if (!tmp)
			return (free(cmd_node), 1);
		arg_arr = split(tmp);
		free(tmp);
	}
	else
		arg_arr = split(cmd_node);
	if (!arg_arr)
		return 1;
	int i = 0; // need to put that loop in a separate function
	while (arg_arr[i])
	{
		char *temp = expand_and_remove(arg_arr[i], app->last_exit_code, app->env_vars);
		if (!temp)
			return (free(cmd_node), ft_free_2d_arr((void **)arg_arr), 1);
		free(arg_arr[i]);
		arg_arr[i] = temp;
		i++;
	}
	if (ast->err_val != 0)
	{
		ft_fprintf(2, "%s: %s: %s\n", NAME, ast->err, strerror(ast->err_val));
		return 1;
	}
	pid = fork();
	if (pid == -1)
	{
		ft_fprintf(2, "%s: fork error: %s\n", NAME, strerror(errno));
		return 1;
	}
	if (pid == 0)
	{
		if (ast->in_fd != STDIN_FILENO)
		{
			dup2(ast->in_fd, STDIN_FILENO);
			close(ast->in_fd);
		}
		if (ast->out_fd != STDOUT_FILENO)
		{
			dup2(ast->out_fd, STDOUT_FILENO);
			close(ast->out_fd);
		}
		fd = 3;
		while (fd < FD_SETSIZE)
		{
			if (fd != STDIN_FILENO && fd != STDOUT_FILENO)
				close(fd);
			fd++;
		}
		if (access(arg_arr[0], X_OK) == 0)
			execve(arg_arr[0], arg_arr, app->env_vars);
		else
			execve(find_path(arg_arr[0], app->env_vars), arg_arr, app->env_vars);
		exit(127);
	}
	else
	{
		if (add_to_pid_list(pid, pid_list))
			return (free(cmd_node), ft_free_2d_arr((void **)arg_arr), 1);
		if (ast->in_fd != 0)
			close(ast->in_fd);
		if (ast->out_fd != 1)
			close(ast->out_fd);
	}
	free(cmd_node);
	ft_free_2d_arr((void **)arg_arr);
	return 0;
}

static int	execute_cmd(char *cmd, char *args, char *ast_args, t_app_data *app);

static int	execute_builtin(t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
{
	int		stdin_fd;
	int		stdout_fd;
	int		exit_code;
	int		pid;
	char	*cmd;
	char	*args;

	cmd = expand_and_remove(ast->cmd, app->last_exit_code, app->env_vars);
	if (!cmd)
		return (1);
	if (ast->args)
	{
		args = expand_and_remove(ast->args, app->last_exit_code, app->env_vars);
		if (!args)
			return (free(cmd), 1);
	}
	else
		args = NULL;
	exit_code = 0;
	if (ast->err_val != 0)
	{
		ft_fprintf(2,"%s: %s: %s\n", NAME, ast->err, strerror(ast->err_val));
		return (1);
	}
	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (ast->in_fd != 0)
	{
		dup2(ast->in_fd, STDIN_FILENO);
		close(ast->in_fd);
	}
	if (ast->out_fd != 1)
	{
		dup2(ast->out_fd, STDOUT_FILENO);
		close(ast->out_fd);
	}
	if (ast->pipe == true)
	{
		pid = fork();
		if (pid == -1)
		{
			ft_fprintf(2, "%s: fork error: %s\n", NAME, strerror(errno));
			return (1);
		}
		if (pid == 0)
		{
			exit_code = execute_cmd(cmd, args, ast->args, app);
			exit(exit_code);
		}
		else
		{
			if (add_to_pid_list(pid, pid_list))
				return (1);
		}
	}
	else
		exit_code = execute_cmd(cmd, args, ast->args, app);
	dup2(stdin_fd, STDIN_FILENO);
	close(stdin_fd);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdout_fd);
	free(cmd);
	if (ast->args)
		free(args);
	return (exit_code);
}

static int	execute_cmd(char *cmd, char *args, char *ast_args, t_app_data *app)
{
	int	exit_code;

	exit_code = 0;
	if (ft_strcmp(cmd, "cd") == 0)
		exit_code = builtin_cd(args);
	else if (ft_strcmp(cmd, "pwd") == 0)
		exit_code = builtin_pwd(args);
	else if (ft_strcmp(cmd, "echo") == 0)
		exit_code = builtin_echo(args, STDOUT_FILENO);
	else if (ft_strcmp(cmd, "env") == 0)
		exit_code = builtin_env(args, &app->env_vars);
	else if (ft_strcmp(cmd, "exit") == 0)
		builtin_exit(app, args);
	else if (ft_strcmp(cmd, "export") == 0)
		exit_code = builtin_export(ast_args, &app->env_vars, STDOUT_FILENO);
	else if (ft_strcmp(cmd, "unset") == 0)
		exit_code = builtin_unset(args, app->env_vars);
	else
		exit_code = 127;
	return (exit_code);
}
=======
>>>>>>> ff9c7fa00235e7e7620e61b21306033764e37de9
