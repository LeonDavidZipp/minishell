/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:56:02 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/06 12:53:30 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// implement cd ~ and cd -
static int	handle_minus(char ***env_vars);

int	builtin_cd(char *path, char ***env_vars, int *last_exit_code)
{
	t_envvar	*new_var;
	char		*temp;

	if (!path)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_fprintf(2, "%s: cd: HOME not set\n", NAME);
			return (1);
		}
	}
	if (path[0] == '-' && ft_strlen(path) == 1)
		return (handle_minus(env_vars));
	if (chdir(path) == -1)
	{
		ft_fprintf(2, "%s: cd: %s: %s\n", NAME, path, strerror(errno));
		return (1);
	}
	temp = ft_sprintf("PWD=%s", getcwd(NULL, 0));
	new_var = split_env_var(temp);
	free(temp);
	*env_vars = update_env_vars(&new_var, last_exit_code, *env_vars);
	return (0);
}


static int	handle_minus(char ***env_vars)
{
	char	*oldpwd;
	char	*pwd;

	oldpwd = ft_getenv("OLDPWD", *env_vars);
	if (!oldpwd)
	{
		ft_fprintf(2, "%s: cd: OLDPWD not set\n", NAME);
		return (1);
	}
	pwd = ft_getenv("PWD", *env_vars);
	if (!pwd)
	{
		ft_fprintf(2, "%s: cd: PWD not set\n", NAME);
		return (1);
	}
	if (chdir(oldpwd) == -1)
	{
		ft_fprintf(2, "%s: cd: %s: %s\n", NAME, oldpwd, strerror(errno));
		return (1);
	}
	return (0);
}
