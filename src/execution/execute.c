/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/07 13:48:02 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	execute_builtin(char *cmd, char *args, char **env_vars);
void		execute_execve(char *cmd, char *args, char **env_vars);

// init_environ file funtion get_path i think is not needed anymore
char	*find_path(char *command, char **envp);

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

int	is_redir(t_tokentype type)
{
	if (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC)
		return (1);
	return (0);
}

void	execute(t_treenode *ast, t_app_data *app)
{
	if (!ast)
		return ;
	// if (ast->cmd_type == PIPE)
	// 	setup_pipe(ast, app);
	// if (is_redir(ast->cmd_type))
	// 	setup_redir(ast, app);
	// if (ast->cmd_type == AND || ast->cmd_type == OR)
	// 	execute_logical(ast, app);
	if (ast->cmd_type == CMD)
	{
		if (is_builtin(ast->cmd))
			execute_builtin(ast->cmd, ast->args, app->env_vars);
		else
			execute_execve(ast->cmd, ast->args, app->env_vars);
	}
	if (ast->left)
		execute(ast->left, app);
	if (ast->right)
		execute(ast->right, app);
}

void	execute_execve(char *cmd, char *args, char **env_vars)
{
	pid_t	pid;
	int		status;
	char	**arg_arr;
	char	*tmp;

	tmp = ft_strjoin(cmd, " ");
	if (!tmp)
		return ;
	if (args)
		arg_arr = ft_split(ft_strjoin(tmp, args), ' ');
	else
		arg_arr = ft_split(tmp, ' ');
	if (!arg_arr)
		return ;
	pid = fork();
	if (pid == 0)
	{
		execve(find_path(cmd, env_vars), arg_arr, env_vars);
		ft_free_2d_arr((void **)arg_arr);
		ft_free_2d_arr((void **)env_vars);
		exit(1);
	}
	else
		waitpid(pid, &status, 0);
	ft_free_2d_arr((void **)arg_arr);
}

static void	execute_builtin(char *cmd, char *args, char **env_vars)
{
	if (ft_strcmp(cmd, "cd") == 0)
		builtin_cd(args);
	else if (ft_strcmp(cmd, "pwd") == 0)
		builtin_pwd(args);
	else if (ft_strcmp(cmd, "echo") == 0)
		builtin_echo(args);
	else if (ft_strcmp(cmd, "env") == 0)
		builtin_env(args, env_vars);
	else if (ft_strcmp(cmd, "exit") == 0)
		builtin_exit(0);
	// else if (ft_strcmp(cmd, "export") == 0)
	// 	builtin_export(args, env_vars);
	// else if (ft_strcmp(cmd, "unset") == 0)
	// 	builtin_unset(args, env_vars);
}

int	main(int argc, char **argv, char **envp)
{
	t_app_data	app;
	t_token		*tokens;
	t_treenode	*root;
	t_treenode	*ast;

	(void)argc;
	(void)argv;
	app.env_vars = init_envp(envp);
	app.input = ft_strdup("env TEST=1 TEST1 =2");
	tokens = tokenize(&app);
	root = combine_cmds_args(tokens);
	ast = NULL;
	ast = build_ast(ast, root, 0);
	debug_printtree(ast, 0);
	execute(ast, &app);
	return (0);
}
