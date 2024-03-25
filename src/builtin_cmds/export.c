/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:06:33 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/25 16:55:12 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_vars(char **env_vars, int fd);

// wrong. when i do export hi hello, export without any arguments lists them, whereas env doesnt list them, meaning this doesnt get actually added to the env vars

// GitHub Copilot
// In Unix-like operating systems, export is a shell built-in function that sets environment variables in the context of the current shell session. When you use export without any arguments, it lists all environment variables in the current shell session, including those you've just added with export.

// On the other hand, env is a standalone command that runs another program in a custom environment. When you run env without any arguments, it prints out the environment variables that would be passed to a program run without any customizations.

// So, if you run export hi=hello and then env, you might not see hi in the output because env is not run in the same shell session where hi was set. But if you run export without any arguments, you should see hi because it's listing the variables in the current shell session.

// In your C code, if you want to add a variable to the environment of the processes that the program spawns, you should use setenv or putenv instead of manipulating env_vars directly. These functions change the environment that's passed to new processes.

// int	builtin_export(char *var_string, char ***env_vars, int fd)
// {
// 	char	***vars;
// 	int		i;

// 	if (!var_string)
// 	{
// 		print_vars(*env_vars, fd);
// 		return (0);
// 	}
// 	vars = split_env_vars(var_string);
// 	if (!vars)
// 		return (1);
// 	i = -1;
// 	while (vars[++i])
// 	{
// 		*env_vars = update_env_vars(vars[i][0], vars[i][1], *env_vars);
// 	}
// 	ft_free_3d_arr((void ***)vars);
// 	return (0);
// }

int	builtin_export(char *var_string, char ***env_vars, int fd)
{
	t_envvar	**vars;
	int			i;

	if (!var_string)
	{
		print_vars(*env_vars, fd);
		return (0);
	}
	vars = split_env_vars(var_string);
	if (!vars)
		return (1);
	i = -1;
	while (vars[++i])
	{
		*env_vars = update_env_vars(vars[i]->key, vars[i]->value,
			vars[i]->includes_equal, *env_vars);
	}
	free_vars(vars);
	return (0);
}

static void	print_vars(char **env_vars, int fd)
{
	t_envvar	*var;
	int			i;

	i = -1;
	while (env_vars[++i])
	{
		var = split_env_var(env_vars[i]);
		ft_putstr_fd("declare -x ", fd);
		ft_putstr_fd(var->key,fd);
		if (var->includes_equal)
			ft_putstr_fd("=", fd);
		if (var->value)
		{
			ft_putstr_fd("\"", fd);
			ft_putstr_fd(var->value, fd);
			ft_putstr_fd("\"", fd);
		}
		ft_putstr_fd("\n", fd);
	}
}

void	free_vars(t_envvar **vars)
{
	int	i;

	i = -1;
	while (vars[++i])
	{
		free(vars[i]->key);
		free(vars[i]->value);
		free(vars[i]);
	}
	free(vars);
}

// int main()
// {
// 	t_env_var	*env_vars;

// 	env_vars = NULL;
// 	update_env_vars("key1", "value1", &env_vars);
// 	update_env_vars("key2", "value2", &env_vars);
// 	update_env_vars("key3", "value3", &env_vars);
// 	update_env_vars("key4", "value4", &env_vars);
// 	// Add two new variables to the environment
// 	builtin_export("VAR1=value1 VAR2=value2", &env_vars);
// 	printf("\n----------------\n");
// 	builtin_export(NULL, &env_vars);
// 	return 0;
// }
