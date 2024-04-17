#include "../../inc/minishell.h"

void	set_error_vars(t_treenode *node, char *err, int val)
{
	node->err_val = val;
	node->err = err;
}

int	execute(t_app_data *app, t_treenode *ast)
{
	t_pid_list	*pid_list;
	int		ret;

	ret = 0;
	pid_list = NULL;
	ret = 0;
	setup_fd(ast, app, &ret);
	if (ret == 2 || g_exit_signal == 2)
	{
		app->last_exit_code = 1;
		return (1);
	}
	// debug_printtree(ast, 0);
	exec_cmds(ast, app, &pid_list);
	wait_and_free(app, &pid_list);
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

void exec_cmds(t_treenode *ast, t_app_data *app, t_pid_list **pid_list)
{
	if (ast == NULL)
		return;
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

void	set_err(t_treenode *node, char *err, int val)
{
	t_treenode	*cmd_node;

	if (!node->left)
	{
		set_error_vars(node, err, val);
		return ;
	}
	cmd_node = find_cmd_node(node->left);
	if (cmd_node->cmd_type != CMD)
		set_error_vars(node, err, val);
	else
		set_error_vars(cmd_node, err, val);
	return ;
}

int	ambigious_redirect(char *str)
{
	int		i;
	bool	s_quote;
	bool	d_quote;

	i = 0;
	s_quote = false;
	d_quote = false;
	while (str[i])
	{
		handle_quotes(str[i], &s_quote, &d_quote);
		if (str[i] == ' ' && !s_quote && !d_quote)
			return (1);
		i++;
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

// splited read_input

static void	init_read_input(int *i, int *exp_flags)
{
	i[0] = -1;
	i[1] = 1;
	exp_flags[0] = 1;
	exp_flags[1] = 0;
	g_exit_signal = 2;
}

char *process_delimiter(char *delimiter, int *i)
{
	char *new_del;
	
	while (delimiter[++i[0]] != '\0')
	{
		if (delimiter[i[0]] == '\'' || delimiter[i[0]] == '"')
			i[1] = 0;
	}
	new_del = remove_quotes(delimiter);
	if (!new_del)
		return (NULL);
	write(0, "> ", 2);
	return (new_del);
}

int check_delimiter(char **line, char *new_del)
{
	char *tmp;
	tmp = ft_strtrim(*line, "\n");
	if (ft_strncmp(tmp, new_del, ft_strlen(new_del)) == 0
		&& ft_strlen(tmp) == ft_strlen(new_del))
	{
		free(tmp);
		return (1);
	}
	return (free(tmp), 0);
}

char	*exp_or_cpy(char *line, int flag, int *exp_flags, t_app_data *app)
{
	char *tmp;

	if (flag == 1)
	{
		tmp = expand(line, app->last_exit_code, app->env_vars, exp_flags);
		if (!tmp)
			return (NULL);
	}
	else
		tmp = ft_strdup(line);
	return (tmp);
}

int	read_input(char *delimiter, int write_fd, t_app_data *app)
{
	char	*line;
	char	*tmp;
	char	*new_del;
	int		i[2];
	int		exp_flags[2];

	init_read_input(i, exp_flags);
	new_del = process_delimiter(delimiter, i);
	line = get_next_line(0);
	while (line != NULL)
	{
		if (check_delimiter(&line, new_del))
			break ;
		tmp = exp_or_cpy(line, i[1], exp_flags, app);
		if (!tmp)
			return (free(line), free(new_del), 1);
		write(write_fd, tmp, ft_strlen(tmp));
		free(tmp);
		free(line);
		write(0, "> ", 2);
		line = get_next_line(0);
	}
	if (line)
		g_exit_signal = 0;
	return (free(line), free(new_del), close(write_fd), 0);
}

void	setup_pipe(t_treenode *node, int *pipe_fd)
{
	t_treenode *cmd_node;

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

int	setup_fd(t_treenode *node, t_app_data *app, int *ret)
{
	int	pipe_fd[2];
	
	if (node->cmd_type == PIPE)
	{
		if (pipe(pipe_fd) == -1)
		{
			ft_fprintf(2, "%s: pipe error: %s\n", NAME, strerror(errno));
			*ret = 2;
			return (2);
		}
		else
			setup_pipe(node, pipe_fd);
	}
	if (is_redir(node->cmd_type))
		setup_redir(node, app);
	if (node->left)
		setup_fd(node->left, app, ret);
	if (node->right)
		setup_fd(node->right, app, ret);
	return (0);
}

