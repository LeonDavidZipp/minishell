/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:41:59 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/20 17:59:51 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// memory leaks!
// norminette!!!!!!!
// need to protect dup2!!!

static int	execute_builtin(t_treenode *ast, t_app_data *app, t_pid_list **pid_list);
static int	execute_execve(t_treenode *ast, char **env_vars, t_pid_list **pid_list);
void		exec_cmds(t_treenode *ast, t_app_data *app, t_pid_list **pid_list);
int			setup_redir(t_treenode *node, int exit_code);
int			setup_fd(t_treenode *node, int exit_code);
char		*find_path(char *command, char **envp);
void		wait_and_free(t_app_data *app, t_pid_list **pid_list);
t_treenode	*find_cmd_node(t_treenode *node);
int			handle_heredoc(t_treenode *node, int exit_code);
int			is_builtin(char *cmd);
int			is_redir(t_tokentype type);

void	set_error_vars(t_treenode *node, char *err)
{
	node->err_val = errno;
	node->err = err;
}

int	execute(t_app_data *app, t_treenode *ast)
{
	t_pid_list	*pid_list;

	pid_list = NULL;
	if (setup_fd(ast, app->last_exit_code) == 2)
		return (1);
	debug_printtree(ast, 0);
	exec_cmds(ast, app, &pid_list);
	wait_and_free(app, &pid_list);
	return (0);
}

void	wait_and_free(t_app_data *app, t_pid_list **pid_list)
{
	t_pid_list	*tmp;
	t_pid_list	*next;
	int			status;

	tmp = *pid_list;
	while (tmp)
	{
		waitpid(tmp->pid, &status, 0);
		if (WIFEXITED(status))
			app->last_exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			app->last_exit_code = 128 + WTERMSIG(status);
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	*pid_list = NULL;
}

int	check_for_errors(t_treenode *ast)
{
	if (ast->err_val != 0)
	{
		printf("Error: %s: %s\n", ast->err, strerror(ast->err_val));
		return 1;
	}
	return 0;
}

void exec_cmds(t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
{
	if (ast == NULL)
		return;
	if (ast->left)
		exec_cmds(ast->left, app, pid_list);
	if (ast->cmd_type == CMD)
	{
		if (is_builtin(ast->cmd))
			app->last_exit_code = execute_builtin(ast, app, pid_list);
		else
			app->last_exit_code = execute_execve(ast, app->env_vars, pid_list);
	}
	else if (ast->cmd_type == PIPE || is_redir(ast->cmd_type))
		app->last_exit_code = check_for_errors(ast);
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

t_treenode *find_cmd_node(t_treenode *node)
{
	while (node->cmd_type != CMD)
	{
		if (node->right)
			node = node->right;
		else if (node->left)
			node = node->left;
		else
			break;
	}
	return (node);
}

int	get_input(char *delimiter, int write_fd, int exit_code)
{
	char	*line;
	char	*expanded;
	char	*tmp;
	char	*new_del;
	bool	should_expand;

	should_expand = true;
	if (delimiter[0] == '\'' || delimiter[0] == '"')
	{
		new_del = remove_quotes(delimiter);
		if (!new_del)
			return (1);
		should_expand = false;
	}
	else
		new_del = delimiter;
	write(0, "> ", 2);
	line = get_next_line(0);
	while (line != NULL)
	{
		tmp = ft_strtrim(line, "\n");
		if (ft_strncmp(tmp, new_del, ft_strlen(new_del)) == 0
			&& ft_strlen(tmp) == ft_strlen(new_del))
			break ;
		free(tmp);
		if (should_expand)
			expanded = expand(line, exit_code, 1);
		else
			expanded = ft_strdup(line);
		if (!expanded)
		{
			free(line);
			return (1);
		}
		write(write_fd, expanded, ft_strlen(expanded));
		free(expanded);
		free(line);
		write(0, "> ", 2);
		line = get_next_line(0);
	}
	free(tmp);
	free(line);
	close(write_fd);
	return (0);
}

int	setup_fd(t_treenode *node, int exit_code)
{
	int	pipe_fd[2];
	t_treenode *cmd_node;
	
	if (node->cmd_type == PIPE)
	{
		if (pipe(pipe_fd) == -1)
		{
			printf("Error: pipe error: %s\n", strerror(errno));
			return (2);
		}
		else
		{
			cmd_node = find_cmd_node(node->left);
			if (cmd_node->cmd_type != CMD)
				close(pipe_fd[1]);
			else
			{
				cmd_node->out_fd = pipe_fd[1];
				cmd_node->pipe = true;
			}
			cmd_node = find_cmd_node(node->right);
			if (cmd_node->cmd_type != CMD)
				close(pipe_fd[0]);
			else
			{
				cmd_node->in_fd = pipe_fd[0];
				cmd_node->pipe = true;
			}
		}
	}
	if (is_redir(node->cmd_type))
		setup_redir(node, exit_code);
	if (node->left)
		setup_fd(node->left, exit_code);
	if (node->right)
		setup_fd(node->right, exit_code);
	return (0);
}

int	handle_heredoc(t_treenode *node, int exit_code)
{
	int			tmp_fd;
	int			pipe_fd[2];
	t_treenode	*cmd_node;

	if (pipe(pipe_fd) == -1)
	{
		if (!node->left)
			return (set_error_vars(node, "heredoc pipe error"), 1);
		cmd_node = find_cmd_node(node->left);
		if (cmd_node->cmd_type != CMD)
			set_error_vars(node, "heredoc pipe error");
		else
			set_error_vars(cmd_node, "heredoc pipe error");
	}
	if (get_input(node->args, pipe_fd[1], exit_code))
	{
		// some error handling like heredoc eror: quote removal or expanding failed
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	tmp_fd = pipe_fd[0];
	if (!node->left)
	{
		close(tmp_fd);
		return (1);
	}
	cmd_node = find_cmd_node(node->left);
	if (cmd_node->cmd_type != CMD)
		return (close(tmp_fd), 1);
	if (cmd_node->in_type == 1)
		close(tmp_fd);
	else
	{
		cmd_node->in_fd = tmp_fd;
		cmd_node->in_type = 2;
	}
	return (0);
}

void	set_err(t_treenode *node)
{
	t_treenode	*cmd_node;

	if (!node->left)
	{
		set_error_vars(node, node->args);
		return ;
	}
	cmd_node = find_cmd_node(node->left);
	if (cmd_node->cmd_type != CMD)
		set_error_vars(node, node->args);
	else
		set_error_vars(cmd_node, node->args);
	return ;
}

void	set_fd(t_treenode *node, int fd, int flag)
{
	t_treenode	*cmd_node;
	
	if (!node->left)
	{
		close(fd);
		return ;
	}
	cmd_node = find_cmd_node(node->left);
	if (cmd_node->cmd_type != CMD)
		close(fd);
	if (flag == 1)
	{
		if (cmd_node->out_type == 1)
			close(fd);
		else
		{
			cmd_node->out_fd = fd;
			cmd_node->out_type = 1;
		}
	}
	else
	{
		if (cmd_node->in_type == 1 || cmd_node->in_type == 2)
			close(fd);
		else
		{
			cmd_node->in_fd = fd;
			cmd_node->in_type = 1;
		}
	}
}

int	setup_redir(t_treenode *node, int exit_code)
{
	int			tmp_fd;

	if (node->cmd_type == REDIR_IN)
	{
		tmp_fd = open(node->args, O_RDONLY);
		if (tmp_fd == -1)
			return (set_err(node), 1);
		else
			set_fd(node, tmp_fd, 2);
	}
	else if (node->cmd_type == HEREDOC)
		return (handle_heredoc(node, exit_code));
	else if (node->cmd_type == REDIR_OUT || node->cmd_type == REDIR_APPEND)
	{
		if (node->cmd_type == REDIR_OUT)
			tmp_fd = open(node->args, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			tmp_fd = open(node->args, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (tmp_fd == -1)
			return (set_err(node), 1);
		else
			set_fd(node, tmp_fd, 1);
	}
	return (0);
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

static int	execute_execve(t_treenode *ast, char **env_vars, t_pid_list **pid_list)
{
	pid_t	pid;
	char	**arg_arr;
	char	*cmd_node;

	cmd_node = ft_strjoin(ast->cmd, " ");
	if (!cmd_node)
		return 1;
	if (ast->args)
		arg_arr = ft_split(ft_strjoin(cmd_node, ast->args), ' ');
	else
		arg_arr = ft_split(cmd_node, ' ');
	if (!arg_arr)
		return 1;
	if (ast->err_val != 0)
	{
		printf("Error: %s: %s\n", ast->err, strerror(ast->err_val));
		return 1;
	}
	pid = fork();
	if (pid == -1)
	{
		printf("Error: fork error: %s\n", strerror(errno));
		return 1;
	}
	if (pid == 0)
	{
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
		execve(find_path(ast->cmd, env_vars), arg_arr, env_vars);
		exit(127);
	}
	else
	{
		if (add_to_pid_list(pid, pid_list))
			return 1;
		if (ast->in_fd != 0)
			close(ast->in_fd);
		if (ast->out_fd != 1)
			close(ast->out_fd);
	}
	ft_free_2d_arr((void **)arg_arr);
	return 0;
}

static int	execute_builtin(t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
{
	int		stdin_fd;
	int		stdout_fd;
	int		exit_code;
	int		pid;

	exit_code = 0;
	if (ast->err_val != 0)
	{
		printf("Error: %s: %s\n", ast->err, strerror(ast->err_val));
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
			printf("Error: fork error: %s\n", strerror(errno));
			return (1);
		}
		if (pid == 0)
		{
			if (ft_strcmp(ast->cmd, "cd") == 0)
				exit_code = builtin_cd(ast->args);
			else if (ft_strcmp(ast->cmd, "pwd") == 0)
				exit_code = builtin_pwd(ast->args);
			else if (ft_strcmp(ast->cmd, "echo") == 0)
				exit_code = builtin_echo(ast->args, STDOUT_FILENO);
			else if (ft_strcmp(ast->cmd, "env") == 0)
				exit_code = builtin_env(ast->args, app->env_vars);
			else if (ft_strcmp(ast->cmd, "exit") == 0)
				builtin_exit(app, 0);
			else if (ft_strcmp(ast->cmd, "export") == 0)
				exit_code = builtin_export(ast->args, &app->env_vars, STDOUT_FILENO);
			else if (ft_strcmp(ast->cmd, "unset") == 0)
				exit_code = builtin_unset(ast->args, app->env_vars);
			exit(exit_code);
		}
		else
		{
			if (add_to_pid_list(pid, pid_list))
				return (1);
		}
	}
	else
	{
		if (ft_strcmp(ast->cmd, "cd") == 0)
			exit_code = builtin_cd(ast->args);
		else if (ft_strcmp(ast->cmd, "pwd") == 0)
			exit_code = builtin_pwd(ast->args);
		else if (ft_strcmp(ast->cmd, "echo") == 0)
			exit_code = builtin_echo(ast->args, STDOUT_FILENO);
		else if (ft_strcmp(ast->cmd, "env") == 0)
			exit_code = builtin_env(ast->args, app->env_vars);
		else if (ft_strcmp(ast->cmd, "exit") == 0)
			builtin_exit(app, 0);
		else if (ft_strcmp(ast->cmd, "export") == 0)
			exit_code = builtin_export(ast->args, &app->env_vars, STDOUT_FILENO);
		else if (ft_strcmp(ast->cmd, "unset") == 0)
			exit_code = builtin_unset(ast->args, app->env_vars);
	}
	dup2(stdin_fd, STDIN_FILENO);
	close(stdin_fd);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdout_fd);
	return (exit_code);
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
	app.input = ft_strdup("ls -l > out > out1");
	app.last_exit_code = 0;
	tokens = tokenize(&app);
	root = combine_cmds_args(tokens);
	root = switch_heredocs(root);
	ast = NULL;
	ast = build_ast(ast, root, 0);
	execute(&app, ast);
	free(app.input);
	return (0);
}
