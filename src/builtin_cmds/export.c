/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:06:33 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/11 13:55:46 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_vars(char **env_vars, int fd);

int	builtin_export(char *var_string, char ***env_vars, int fd)
{
	char	***vars;
	char	**temp;
	int		i;

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
		temp = update_env_vars(vars[i][0], vars[i][1], *env_vars);
		if (!temp)
		{
			ft_free_3d_arr((void ***)vars);
			return (1);
		}
		ft_free_2d_arr((void **)temp);
	}
	ft_free_3d_arr((void ***)vars);
	return (0);
}

static void	print_vars(char **env_vars, int fd)
{
	char	**key_value;
	int		i;

	i = -1;
	while (env_vars[++i])
	{
		key_value = split_env_var(env_vars[i]);
		ft_putstr_fd("declare -x ", fd);
		ft_putstr_fd(key_value[0],fd);
		ft_putstr_fd("=", fd);
		if (key_value[1])
		{
			ft_putstr_fd("\"", fd);
			ft_putstr_fd(key_value[1], fd);
			ft_putstr_fd("\"", fd);
		}
		ft_putstr_fd("\n", fd);
	}
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
