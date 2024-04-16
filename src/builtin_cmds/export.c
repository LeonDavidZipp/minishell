/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:06:33 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/16 11:46:51 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_vars(char **env_vars, int fd);

int	builtin_export(char *var_string, t_app_data **app_data, int fd)
{
	t_envvar	**vars;
	int			i;
	int			exit_code;

	exit_code = 0;
	if (!var_string)
	{
		print_vars((*app_data)->env_vars, fd);
		return (exit_code);
	}
	vars = split_env_vars(var_string, app_data);
	if (!vars)
		return (1);
	i = -1;
	while (vars[++i])
	{
		(*app_data)->env_vars = update_env_vars(&(vars[i]), &exit_code,
				(*app_data)->env_vars);
	}
	free_vars(vars);
	return (exit_code);
}

static void	print_vars(char **env_vars, int fd)
{
	t_envvar	*var;
	int			i;

	i = -1;
	while (env_vars[++i])
	{
		var = split_env_var(env_vars[i]);
		ft_fprintf(fd, "declare -x %s", var->key);
		if (var->includes_equal)
			ft_fprintf(fd, "=");
		if (var->value)
			ft_fprintf(fd, "\"%s\"", var->value);
		ft_fprintf(fd, "\n");
		free(var->key);
		free(var->value);
		free(var);
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
